#include "entity.hh"
extern "C" {
#include <libcd.h>
}
// 1 (Open)
// 3 (GetSizeSth (0, 0) only get size flag)
// 3 (GetSizeSth (0, 2))
// 5 (Read)
// 2 (Close)

// destructor:
// 13
// 7

class File : public Entity {
public:
    virtual int Kind() const override { return 0x3; }

    // these two are weird in the original. they have CDFile stuff.
    File();
    virtual ~File();

    /*  0 */ virtual void Init() = 0;
    /*  1 */ virtual void Open(char* filename, int, int) = 0;
    /*  2 */ virtual void Close() = 0;
    /*  3 */ virtual int GetSize(int, int) = 0; // Stat?
    /*  4 */ virtual void Func4() = 0;
    /*  5 */ virtual void Read(void* dst, int size) = 0;
    /*  6 */ virtual void ReadAll(char* filename);
    /*  7 */ virtual void FreeBuffer();
    /*  8 */ virtual void DoNothing0();
    /*  9 */ virtual void SetFlag();
    /* 10 */ virtual void Func10() = 0;
    /* 11 */ virtual void Func11(char* fileName) = 0;   // entry point
    /* 12 */ virtual void Func12() = 0;
    /* 13 */ virtual void Func13() = 0;

    //
    static void AdvanceArray(int n);
    static void SetCurrDir(char* dir);
    static char* GetCurrDir();
    // some string function
    
protected:
    bool m_Open;
    void* m_Buffer;
    int m_Size;
    
    // it appears that this really does have the CD fields. which is weird.
    CdlLOC m_Pos;
    uint m_DiscSize;
    short m_Unk0;
    short m_CmdCount;
    int m_Flags;
    short m_DontQueue;    // Actually Run
    short m_Unk2;   // padding?

private:
    static char* s_CurrDir;

};