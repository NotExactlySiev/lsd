#include "file.hh"

//
File::File() : Entity() {
    m_Found = false;
    m_Buffer = nullptr;
    m_Size = 0;
    m_Unk0 = 0;
    m_CmdCount = 0;
    m_Flags = 0;
    m_Unk1 = 0;
    m_Unk2 = 0;
}

File::~File() {
    Close();
    FreeBuffer();
}

void File::ReadAll(char *fileName) {
    if (m_Buffer)
        return;
    
    bool found = m_Found;
    m_Found = false;
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
    m_Found = found;
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