#include "file.hh"

class CDFile : public File {
public:
    virtual int Kind() const override { return 0x13; }

    // these two are weird in the original. they have CDFile stuff.
    CDFile();
    virtual ~CDFile() noexcept;
    
    /*  0 */ virtual void Init() override;
    /*  1 */ virtual void Open(char* filename, int, int) override;
    /*  2 */ virtual void Close() override;
    /*  3 */ virtual int GetSize(int, int) override; // Stat?
    /*  4 */ virtual void Func4() override;
    /*  5 */ virtual void Read(void* dst, int size) override;
    /*  6 */ virtual void ReadAll(char* fileName) override;
    /*  7 */ virtual void FreeBuffer();
    /*  8 */ virtual void Nothing0();
    /*  9 */ virtual void SetFlag();
    /* 10 */ virtual void Func10() override;    // static?
    /* 11 */ virtual void Func11(char* fileName) override;
    /* 12 */ virtual void Func12() override;
    /* 13 */ virtual void Func13() override;

    static void SetFastMode();
    static bool SetOptions(int option0, int option1, int option2);

private:
    static void Lock();
    static void Unlock();
    
    struct Command;
    static Command* s_Queue;
    
    struct Command {
        // I don't think these two functions were ctor/dtor in the original.
        // but this is neater and basically the same thing.
        Command();
        ~Command();

        int m_Active;
        int m_Unk0;
        int m_Code;
        CDFile* m_File;
        int m_FileIndex;
        int m_Unk1;
        int m_Unk2;
        Command* m_Prev;
        Command* m_Next;        
    };

    static void Callback();
    //
    //
    //
    static void StartRunning();
    void IssueCommand(int fileIndex, int cmd, int arg0, int arg1);

    static bool s_Lock;
    static bool s_ModeSet;
    static int s_Option0;
    static int s_Option1;
    static bool s_SomeCheck;
    //
    static int s_Option2;
    
};