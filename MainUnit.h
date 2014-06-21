//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
//#define RUN_FROM_IDE
#define DEFAULT_TO_PYTHON
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
//---------------------------------------------------------------------------
#include "AddDirectoryUnit.h"
#include "ScriptEngine.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog;
	TMainMenu *MainMenu;
	TMenuItem *FileMenu;
	TMenuItem *AddFilesMenu;
	TMenuItem *AddFoldersMenu;
	TMenuItem *N2;
	TMenuItem *ClearListMenu;
	TMenuItem *LoadListMenu;
	TMenuItem *SaveListMenu;
	TMenuItem *N3;
	TMenuItem *CloseMenu;
	TMenuItem *ScriptMenu;
	TMenuItem *Language1;
	TMenuItem *PythonMenu;
	TMenuItem *LuaMenu;
	TMenuItem *N4;
	TMenuItem *LoadScriptMenu;
	TMenuItem *SaveScriptMenu;
	TMenuItem *N1;
	TMenuItem *TestScriptMenu;
	TMenuItem *ExecuteScriptMenu;
	TMenuItem *OptionsMenu;
	TMenuItem *HidePathsMenu;
	TListView *ListViewFilesList;
	TPanel *ScriptPanel;
	TSplitter *Splitter1;
	TPanel *ScriptOutputPanel;
	TRichEdit *RichEditScriptOutput;
	TPanel *ScriptTextPanel;
	TRichEdit *RichEditScriptText;
	TSplitter *Splitter2;
	TStatusBar *StatusBar;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TabControlChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CloseMenuClick(TObject *Sender);
	void __fastcall HidePathsMenuClick(TObject *Sender);
	void __fastcall AddFilesMenuClick(TObject *Sender);
	void __fastcall AddFoldersMenuClick(TObject *Sender);
	void __fastcall LoadListMenuClick(TObject *Sender);
	void __fastcall SaveListMenuClick(TObject *Sender);
	void __fastcall LoadScriptMenuClick(TObject *Sender);
	void __fastcall SaveScriptMenuClick(TObject *Sender);
	void __fastcall TestScriptMenuClick(TObject *Sender);
	void __fastcall ExecuteScriptMenuClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ListViewFilesListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LuaMenuClick(TObject *Sender);
	void __fastcall ClearListMenuClick(TObject *Sender);

private:	// User declarations
	AnsiString LuaPlugin, PythonPlugin;
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	bool __fastcall LoadSettings();
	bool __fastcall SaveSettings();
	void __fastcall InitializeUI();
	void __fastcall UpdateLeftStatusbarMsg();
	void __fastcall UpdateRightStatusbarMsg(int NumRenamed, int TotalItems);
	bool __fastcall IsEntryDuplicated(UnicodeString FileName);
	void __fastcall AddListEntry(UnicodeString FileName);
	void __fastcall ClearSelectedEntries();
	void __fastcall RecurseDirectory(char *Dir, bool Recurse);
	bool __fastcall RunScript(bool Simulate);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
