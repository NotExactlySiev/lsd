#include "file.hh"

struct FileAddr {
    char path[16];
    //
    CdlLOC pos;
    int size;
};

class CDFile : public File {
public:
    virtual int Kind() const override { return 0x13; }

    // these two are weird in the original. they have CDFile stuff.
    CDFile();
    virtual ~CDFile() noexcept;
    
    /*  0 */ virtual void Init() override;
    /*  1 */ virtual void Open(char* filename, int arg0, int arg1) override;
    /*  2 */ virtual void Close() override;
    /*  3 */ virtual int GetSize(int, int) override; // Stat?
    /*  4 */ virtual void Func4() override;
    /*  5 */ virtual void Read(void* dst, int size) override;
    /*  6 */ virtual void ReadAll(char* fileName) override;
    /*  7 */ virtual void FreeBuffer();
    /*  8 */ virtual void Nothing0();
    /*  9 */ virtual void SetFlag();
    /* 10 */ virtual void Func10() override;
    /* 11 */ virtual void Func11(char* fileName) override;  // entry point
    /* 12 */ virtual void Func12() override;
    /* 13 */ virtual void Func13() override;

    static void SetFastMode();
    static bool SetOptions(int option0, int option1, int option2);

private:
    void SimpleOpen(char* fileName);
    static char* MakeFullAddr(char* dst, char* fileName);
    void SimpleClose();
    int SimpleSize();
    int SimpleRead(void* buffer, uint size);

    static void Lock();
    static void Unlock();
    
    struct Command;
    
    
    struct Command {
        // I don't think these two functions were ctor/dtor in the original.
        // but this is neater and basically the same thing.
        explicit Command();
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
    static void StartRunningCommands();
    void IssueCommand(int fileIndex, int cmd, int arg0, int arg1);

    
    //
    static bool s_ModeSet;
    static int s_Option0;
    static int s_Option1;
    static bool s_PrimaryRunning;
    // s_AddrArray
    static int s_CurrIndex;
    static int s_LetSecondaryEnd;
    static int s_Unused0;
    static int s_PrimaryStep;
    static FileAddr* s_CurrAddr;
    static int s_CurrSectorCount;
    static void* s_CurrBuffer;
    static FileAddr* s_NextAddr;
    static bool s_Lock;
    static int s_RunningQueue;
    static Command* s_Queue;
    static int s_PrimaryCommand;
    static int s_RunningCommands;
    static int s_PrimaryCounter;
    static int s_Option2;


    // unordered
    static FileAddr* GetFile(char*);
    static int GetFileIndex(char*);
    static FileAddr* GetFromArray(int);
    static void RunPrimary1();
    static void RunPrimary2();
    static void SetPrimaryStatus(int, int);
    static void StopAll();  // call when successfully completed primary?
    static void SetPrimaryStep(int);
    
};