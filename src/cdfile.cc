#include "cdfile.hh"

CDFile::CDFile() : File() {
    m_Unk1 = 0;
    SetFastMode();
}

CDFile::~CDFile() {
    Func13();
    FreeBuffer();
}

void CDFile::Init() {}



//

void CDFile::SetFastMode() {
    if (!s_ModeSet) {
        CdSetDebug(0);
        u_char param[8] = { 0x80 };
        while (0 == CdControlB(CdlSetmode, param, nullptr));
        s_ModeSet = true;
    }
}

//
//

void CDFile::Lock() {
    s_Lock = true;
}

void CDFile::Unlock() {
    s_Lock = false;
}

// Callback

// StartRunning

// DoFunc12

//

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

//