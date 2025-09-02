#include "file.hh"

//
File::File() : Entity() {
    m_Open = false;
    m_Buffer = nullptr;
    m_Size = 0;
    m_Unk0 = 0;
    m_CmdCount = 0;
    m_Flags = 0;
    m_DontQueue = 0;
    m_Unk2 = 0;
}

File::~File() {
    Close();
    FreeBuffer();
}

void File::ReadAll(char *fileName) {
    if (m_Buffer)
        return;
    
    bool found = m_Open;
    m_Open = false;
    Open(fileName, 1, 0);
    int size = GetSize(0, 2);
    void* buf = new char[size];
    if (buf == nullptr) {
        delete buf; // lol
        Close();
        return;
    }

    GetSize(0, 0);
    Read(buf, size);
    Close();
    m_Buffer = buf;
    m_Size = size;
    m_Open = found;
}

void File::FreeBuffer() {
    if (m_Buffer && m_Size != 0 && !m_Unk0) {
        delete m_Buffer;
        m_Buffer = nullptr;
    }
}

void File::DoNothing0() {}

void File::SetFlag() {
    m_Flags |= 1;
}

// vt fuckery here
// also ModelSetParam ctor for some reason?

//void File::Lock() {}
//void File::Unlock() {}

// unused stuff

//void File::SetOptions() {}

//

void File::AdvanceArray(int n) {
    // TODO
}


void File::SetCurrDir(char* dir) {
    s_CurrDir = dir;
}

char* File::GetCurrDir() {
    return s_CurrDir;
}

//