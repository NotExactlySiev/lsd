#include "cdfile.hh"
#include "gpu.hh"
extern "C" {
#include <stdio.h>
#include <string.h>
}

CDFile::CDFile() : File() {
    m_DoItNow = 0;
    SetFastMode();
}

CDFile::~CDFile() {
    Func13();
    FreeBuffer();
}

void CDFile::Init() {}

void CDFile::Open(char* filename, int arg0, int arg1) {
    if (!s_Option0 && !s_Option1) {
        SimpleOpen(filename);
        return;
    }

    Lock();
    if (!m_DoItNow) {
        IssueCommand(GetFileIndex(filename), 2, arg0, arg1);
        goto out;
    }

    if (s_SomeCheck || m_Open) {
        goto out;
    }

    if (s_Option0) {
        //s_CurrFile =
        //
        //
        s_PrimaryCommand = 1;
        m_Open = true;
        //
    } else {
        int rc;
        char path[16];
        CdlFILE file;

        SetPrimaryStatus(1, 1);
        MakeFullAddr(path, filename);
        while (CdSearchFile(&file, path) == nullptr);
        m_Pos = file.pos;
        m_DiscSize = file.size;

        do {
            CdControl(CdlSetloc, (u_char*) &m_Pos, nullptr);
            while ((rc = CdSync(0, nullptr)) == CdlNoIntr);
        } while (rc == CdlDiskError);
        m_Open = true;
        StopAll();
    }
out:
    Unlock();
}

//

// non virtuals

void CDFile::SetFastMode() {
    if (!s_ModeSet) {
        CdSetDebug(0);
        u_char param[8] = { 0x80 };
        while (0 == CdControlB(CdlSetmode, param, nullptr));
        s_ModeSet = true;
    }
}

bool CDFile::SetOptions(int option0, int option1, int option2) {
    if (s_SomeCheck)
        return false;

    if (option2 == 0 && option0 != s_Option0) {
        GPU::GetGlobal()->SetCallback(s_Option0 ? nullptr : Callback);
    }

    s_Option0 = option0;    // no callback
    s_Option1 = option1;
    s_Option2 = option2;
    return true;
}

// SetArr
// SetIndex
// GetIndex

void CDFile::Lock() {
    s_Lock = true;
}

void CDFile::Unlock() {
    s_Lock = false;
}

// Callback

// StartRunning

// StopRunning

void CDFile::IssueCommand(int fileIndex, int cmd, int arg0, int arg1) {
    Command* command = new Command;
    command->m_Code = cmd;
    command->m_Unk1 = arg0;
    command->m_File = this;
    command->m_FileIndex = fileIndex;
    command->m_Unk2 = arg1;
    m_Flags = 0;
    m_CmdCount++;
    StartRunning();
}

CDFile::Command::Command() {
    Lock();
    m_Prev = nullptr;
    m_Next = nullptr;
    m_Active = 0;
    m_Unk0 = 0;
    
    if (s_Queue) {
        Command* last = s_Queue;
        while (last->m_Next) {
            last = last->m_Next;
        }
        last->m_Next = this;
        m_Next = last;
    } else {
        s_Queue = this;
    }
    Unlock();
}

CDFile::Command::~Command() {
    Lock();
    if (m_Prev) {
        m_Prev->m_Next = m_Next;
    } else {
        s_Queue = m_Next;
    }
    if (m_Next) {
        m_Next->m_Prev = m_Prev;
    }
    Unlock();
}

// GetFile
// GetFile
// GetFile

/*
    static void RunPrimary1();
    static void RunPrimary2();
    static void SetPrimaryStatus(int, int);
    static void StopAll();
    static void SetPrimaryState(int, int);
*/

//

void CDFile::SimpleOpen(char* fileName) {
    if (m_Open)
        return;
    
    char path[64];
    MakeFullAddr(path, fileName);
    for (int to = 0; to < 100; to++) {
        CdlFILE file;
        if (CdSearchFile(&file, path)) {
            m_Pos = file.pos;
            m_DiscSize = file.size;
            m_Open = true;
            return;
        }
    }

    printf("File not found. path = %s\n", path);
}

char* CDFile::MakeFullAddr(char* dst, char* fileName) {
    dst[0] = '\\';
    strcpy(dst + 1, GetCurrDir());
    strcat(dst, fileName);
    strcat(dst, ";1");
    return dst;
}

void CDFile::SimpleClose() {
    if (m_Open) {
        m_Open = false;
    }
}

int CDFile::SimpleSize() {
    if (m_Open) {
        return (m_DiscSize / 2048 + 1) * 2048;
    } else {
        return 0;
    }
}

int CDFile::SimpleRead(void* buffer, uint size) {
    if (!m_Open) {
        Close();
        return 0;
    }

    int rc;
    u_char result[16];

    do {
        do {
            CdControl(CdlSetloc, (u_char*) &m_Pos, nullptr);
            while ((rc = CdSync(0, result)) == CdlNoIntr);
        } while (rc == CdlDiskError);

        if (size / 2048 == 0)
            return 0;

        CdRead(size / 2048, (u_long*) buffer, CdlModeSpeed);
        while ((rc = CdReadSync(0, nullptr)) > 0);
    } while (rc == -1);
    return 0;
}

// end