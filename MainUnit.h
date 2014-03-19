//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
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
#include "AddDirectoryUnit.h"
#include "ScriptEngine.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *FileMenu;
	TMenuItem *AddFilesMenu;
	TMenuItem *AddFoldersMenu;
	TMenuItem *ScriptMenu;
	TMenuItem *LoadScriptMenu;
	TMenuItem *SaveScriptMenu;
	TMenuItem *N1;
	TMenuItem *ExecuteScriptMenu;
	TMenuItem *TestScriptMenu;
	TMenuItem *N2;
	TMenuItem *LoadListMenu;
	TMenuItem *SaveListMenu;
	TMenuItem *N3;
	TMenuItem *OptionsMenu;
	TMenuItem *HidePathsMenu;
	TMenuItem *CloseMenu;
	TTabControl *TabControl;
	TPanel *FilesListPanel;
	TListView *ListViewFilesList;
	TPanel *ScriptPanel;
	TPanel *ScriptTextPanel;
	TPanel *ScriptOutputPanel;
	TSplitter *Splitter1;
	TRichEdit *RichEditScriptText;
	TRichEdit *RichEditScriptOutput;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TMenuItem *Language1;
	TMenuItem *N4;
	TMenuItem *PythonMenu;
	TMenuItem *LuaMenu;
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

private:	// User declarations
	AnsiString LuaPlugin, PythonPlugin;
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	void __fastcall InitializeUI();
	bool __fastcall IsEntryDuplicated(UnicodeString FileName);
	void __fastcall AddListEntry(UnicodeString FileName);
	void __fastcall RecurseDirectory(char *Dir, bool Recurse);
	bool __fastcall RunScript(bool Simulate);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
