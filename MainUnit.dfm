object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Batch Files Renamer'
  ClientHeight = 576
  ClientWidth = 1136
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter2: TSplitter
    Left = 529
    Top = 0
    Height = 557
    ExplicitHeight = 576
  end
  object ListViewFilesList: TListView
    Left = 0
    Top = 0
    Width = 529
    Height = 557
    Align = alLeft
    Columns = <
      item
        AutoSize = True
        Caption = 'Original'
      end
      item
        AutoSize = True
        Caption = 'Original'
      end
      item
        AutoSize = True
        Caption = 'Modified'
      end>
    GridLines = True
    MultiSelect = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnKeyDown = ListViewFilesListKeyDown
  end
  object ScriptPanel: TPanel
    Left = 532
    Top = 0
    Width = 604
    Height = 557
    Align = alClient
    Color = clTeal
    ParentBackground = False
    TabOrder = 1
    object Splitter1: TSplitter
      Left = 1
      Top = 345
      Width = 602
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 3
      ExplicitTop = 353
    end
    object ScriptOutputPanel: TPanel
      Left = 1
      Top = 348
      Width = 602
      Height = 208
      Align = alClient
      Color = clOlive
      ParentBackground = False
      TabOrder = 0
      object RichEditScriptOutput: TRichEdit
        Left = 1
        Top = 1
        Width = 600
        Height = 206
        Align = alClient
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object ScriptTextPanel: TPanel
      Left = 1
      Top = 1
      Width = 602
      Height = 344
      Align = alTop
      Color = clOlive
      ParentBackground = False
      TabOrder = 1
      object RichEditScriptText: TRichEdit
        Left = 1
        Top = 1
        Width = 600
        Height = 342
        Align = alClient
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 557
    Width = 1136
    Height = 19
    Panels = <
      item
        Width = 250
      end
      item
        Width = 50
      end>
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 807
    Top = 33
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 871
    Top = 33
  end
  object MainMenu: TMainMenu
    Left = 936
    Top = 32
    object FileMenu: TMenuItem
      Caption = 'File'
      object AddFilesMenu: TMenuItem
        Caption = 'Add Files...'
        OnClick = AddFilesMenuClick
      end
      object AddFoldersMenu: TMenuItem
        Caption = 'Add Folders...'
        OnClick = AddFoldersMenuClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object ClearListMenu: TMenuItem
        Caption = 'Clear List'
        OnClick = ClearListMenuClick
      end
      object LoadListMenu: TMenuItem
        Caption = 'Load List...'
        OnClick = LoadListMenuClick
      end
      object SaveListMenu: TMenuItem
        Caption = 'Save List...'
        OnClick = SaveListMenuClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object CloseMenu: TMenuItem
        Caption = 'Close'
        OnClick = CloseMenuClick
      end
    end
    object ScriptMenu: TMenuItem
      Caption = 'Script'
      object Language1: TMenuItem
        Caption = 'Language'
        object PythonMenu: TMenuItem
          Tag = 1
          Caption = 'Python'
          Checked = True
          OnClick = LuaMenuClick
        end
        object LuaMenu: TMenuItem
          Caption = 'Lua'
          OnClick = LuaMenuClick
        end
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object LoadScriptMenu: TMenuItem
        Caption = 'Load...'
        OnClick = LoadScriptMenuClick
      end
      object SaveScriptMenu: TMenuItem
        Caption = 'Save...'
        OnClick = SaveScriptMenuClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object TestScriptMenu: TMenuItem
        Caption = 'Test Script'
        OnClick = TestScriptMenuClick
      end
      object ExecuteScriptMenu: TMenuItem
        Caption = 'Execute Script'
        OnClick = ExecuteScriptMenuClick
      end
    end
    object OptionsMenu: TMenuItem
      Caption = 'Options'
      object HidePathsMenu: TMenuItem
        Caption = 'Hide Paths'
        Checked = True
        OnClick = HidePathsMenuClick
      end
    end
  end
end
