#include "cdfile.hh"
#include "gpu.hh"
extern "C" {
#include <stdio.h>
#include <string.h>
#include <libetc.h>
}

bool CDFile::s_ModeSet = false;
int CDFile::s_Option0 = 0;
int CDFile::s_Option1 = 0;
bool CDFile::s_PrimaryRunning = 0;
int CDFile::s_CurrIndex = 0;
int CDFile::s_LetSecondaryEnd = 1;
int CDFile::s_Unused0 = 0;
int CDFile::s_PrimaryStep = 0;
FileAddr* CDFile::s_CurrAddr = nullptr;
int CDFile::s_CurrSectorCount = 0;
void* CDFile::s_CurrBuffer = nullptr;
FileAddr* CDFile::s_NextAddr = nullptr;
bool CDFile::s_Lock = false;
int CDFile::s_RunningQueue = 0;
CDFile::Command* CDFile::s_Queue = nullptr;
int CDFile::s_PrimaryCommand = 0;
int CDFile::s_RunningCommands = 0;
int CDFile::s_PrimaryCounter = 0;
int CDFile::s_Option2 = 1;

CDFile::CDFile() : File() {
    m_AllowBlocking = 0;
    SetFastMode();
}

CDFile::~CDFile() {
    Func13();
    FreeBuffer();
}

void CDFile::Init() {}

void CDFile::Open(char* filename, int arg0, int arg1) {
    if (!s_Option0 && !s_Option1) {
        SimpleOpen(filename);   // dummy?
        return;
    }

    Lock();
    if (!m_AllowBlocking) {   // queue it if we can't block
        IssueCommand(GetFileIndex(filename), Command::Code::Open, arg0, arg1);
        goto out;
    }

    if (s_PrimaryRunning || m_Open) {
        goto out;
    }

    if (s_Option0) {
        // async
        s_CurrAddr = GetFile(filename);
        if (s_CurrAddr == nullptr)
            return;
        m_Pos = s_CurrAddr->pos;
        s_PrimaryCommand = 1;
        m_Open = true;
        m_DiscSize = s_CurrAddr->size;
    } else {
        // block
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

void CDFile::Close() {
    if (!s_Option0 && !s_Option1) {
        SimpleClose();
        return;
    }
    Lock();
    if (!m_AllowBlocking) {
        IssueCommand(0, Command::Code::Close, 0, 0);
    } else if (!s_PrimaryRunning) {
        SetPrimaryStatus(0, 0);
        m_Open = false;
        StopAll();
    }
    Unlock();
}

//
//

void CDFile::Read(void* dst, int size) {
    if (!s_Option0 && !s_Option1) {
        SimpleRead(dst, size);
        return;
    }

    Lock();

    if (!m_AllowBlocking) {   // allow blocking?
        IssueCommand(0, Command::Code::Read, (int) dst, size);
        goto out;
    }

    if (s_PrimaryRunning || !m_Open) {
        goto out;
    }

    SetPrimaryStatus(3, 7);
    if (s_Option0) {
        // async
        s_PrimaryCommand = 1;
        s_CurrSectorCount = size / 2048;
        s_CurrBuffer = dst;
        //
    } else {
        // block
        CdRead(size / 2048, (u_long*) dst, CdlModeSpeed);
        int rc;
        do {
            while ((rc = CdReadSync(0, nullptr)) > 0);
        } while (rc == -1);
        StopAll();
    }
out:
    Unlock();
}

void CDFile::ReadAll(char* fileName) {
    if (!s_Option0 && !s_Option1) {
        File::ReadAll(fileName);
        m_Flags |= 0x200;
        SetFlag();
    } else {
        //
    }

}

//

// RunFromQueue
void CDFile::Func10() {
    CDFile* file;

    Lock();
    Command* cmd = s_Queue;
    if (cmd == nullptr) goto out;
    file = cmd->m_File;
    if (cmd->m_Active) {
        if (!s_LetSecondaryEnd) goto out;
        if (s_Queue->m_Unk0)
            file->m_Flags |= 1;
        file->m_CmdCount -= 1;
        file->m_Flags |= 2;
        if (file->m_CmdCount == 0)
            file->m_Flags |= 4;
        switch (s_Queue->m_Code) {
            case Command::Code::Open: file->m_Flags |= 0x10; break;
            case Command::Code::Close: file->m_Flags |= 0x20; break;
            case Command::Code::GetSize: file->m_Flags |= 0x40; break;
            case Command::Code::Read: file->m_Flags |= 0x80; break;
            case Command::Code::ReadAll: file->m_Flags |= 0x200; break;
        }
        file->SetFlag();
        delete s_Queue; // hidden control flow!
        if (s_Queue == nullptr) {
            file->Func12();
        }
    } else {
        file->m_AllowBlocking = 1;
        switch (cmd->m_Code) {
            case Command::Code::Open: {
                FileAddr* addr = GetFromArray(cmd->m_FileIndex);
                file->Open(addr->path, cmd->m_Unk1, cmd->m_Unk2);
            } break;

            case Command::Code::Close: {
                file->Close();
            } break;

            case Command::Code::GetSize: {
                file->GetSize(cmd->m_Unk1, cmd->m_Unk2);
            } break;

            case Command::Code::Read: {
                file->Read((void*) cmd->m_Unk1, cmd->m_Unk2);
            } break;

            case Command::Code::ReadAll: {
                FileAddr* addr = GetFromArray(cmd->m_FileIndex);
                file->ReadAll(addr->path);
            } break;
        }
        file->m_AllowBlocking = 0;
    }

out:
    Unlock();
}

void CDFile::Func11(char* fileName) {
    Lock();
    if (fileName) {
        if (s_Option0) {
            IssueCommand(GetFileIndex(fileName), Command::Code::ReadAll, 0, 0);
        } else {
            ReadAll(fileName);
            if (m_CmdCount == 0) {
                m_Flags |= 4;
            }
        }
    }
    Unlock();
}

void CDFile::Func12() {
    Lock();
    //
    //Locked...
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
    if (s_PrimaryRunning)
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

bool get_allocator_lock();

// Callback
void CDFile::Callback() {
    if (s_Lock || get_allocator_lock()) return;
    if (s_Option2)
        VSyncCallback(nullptr);
    if (s_PrimaryCommand == 1)
        RunPrimary1();
    else if (s_PrimaryCommand == 2)
        RunPrimary2();
    if (s_RunningQueue) {
        CDFile tmp; // !!! I swear to christ
        tmp.Func10();
    }
    if (s_Option2)
        VSyncCallback(Callback);
}

// StartRunning
void CDFile::StartRunningCommands() {
    Lock();
    if (s_RunningCommands == 0) {
        if (s_Option2)
            VSyncCallback(Callback);
        s_RunningCommands = 1;
    }
    s_RunningQueue = 1;
    Unlock();
}

// StopRunning

// enqueue
void CDFile::IssueCommand(int fileIndex, Command::Code code, int arg0, int arg1) {
    Command* command = new Command;
    command->m_Code = code;
    command->m_Unk1 = arg0;
    command->m_File = this;
    command->m_FileIndex = fileIndex;
    command->m_Unk2 = arg1;
    m_Flags = 0;
    m_CmdCount++;
    StartRunningCommands();
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

//


//FileAddr* CDFile::GetFile(char*) {
    //
    //
//}

// GetFile
// GetFile

void CDFile::RunPrimary1() {
    Lock();

    int rc;
    int next;
    switch (s_PrimaryStep) {
        case 1: {
            rc = CdControlF(CdlSetloc, (u_char*) &s_CurrAddr->pos);
            next = 2;
        } break;

        case 2: {
            rc = CdSync(1, nullptr);
            switch (rc) {
                case CdlNoIntr: {
                    next = 1;
                    rc = s_PrimaryCounter++ > 600;
                } break;

                case CdlDataReady: {
                    // nothing
                    rc = 0;
                } break;

                case CdlDiskError: {
                    next = 1;
                } break;

                case CdlComplete:
                    StopAll();
                default:
                    rc = 0;
                    break;
            }
        } break;

        case 7: {
            rc = CdRead(s_CurrSectorCount, (u_long*) s_CurrBuffer, CdlModeSpeed);
            next = 8;
        } break;

        case 8: {
            rc = CdReadSync(1, nullptr);
            if (rc == -1) {
                CdFlush();
                next = 1;
            } else if (rc != 0) {
                StopAll();
            }
        } break;
    }

    if (rc) {
        SetPrimaryStep(next);
    }
out:
    Unlock();
}

void CDFile::RunPrimary2() {
    int rc;
    int next;
    Lock();
    switch (s_PrimaryStep) {
        case 1: {
            rc = CdControlF(CdlSetloc, (u_char*) &s_CurrAddr->pos);
            next = 2;
        } break;

        case 2: {
            //
        } break;

        case 7: {

        } break;

        case 8: {
            rc = CdReadSync(1, nullptr);
            next = 1;
            if (rc == -1) {
            } else if (rc != 0) {
                rc = 0;
            } else {
                StopAll();
                s_CurrAddr = s_NextAddr;
            }
        } break;
    }

out:
    Unlock();
}

/*
    
    void CDFile::SetPrimaryStatus(int, int)
    void CDFile::StopAll()
    
*/

void CDFile::SetPrimaryStep(int val) {
    s_PrimaryStep = val;
    s_PrimaryCounter = 0;
}

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