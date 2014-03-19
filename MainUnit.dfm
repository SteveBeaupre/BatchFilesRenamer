object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Batch Files Renamer'
  ClientHeight = 503
  ClientWidth = 808
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
  object TabControl: TTabControl
    Left = 0
    Top = 0
    Width = 808
    Height = 503
    Align = alClient
    TabOrder = 0
    Tabs.Strings = (
      'Files'
      'Script')
    TabIndex = 0
    OnChange = TabControlChange
    object FilesListPanel: TPanel
      Left = 39
      Top = 48
      Width = 362
      Height = 353
      Color = clActiveCaption
      ParentBackground = False
      TabOrder = 0
      object ListViewFilesList: TListView
        Left = 16
        Top = 16
        Width = 329
        Height = 289
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
    end
    object ScriptPanel: TPanel
      Left = 422
      Top = 48
      Width = 362
      Height = 353
      Color = clTeal
      ParentBackground = False
      TabOrder = 1
      object Splitter1: TSplitter
        Left = 1
        Top = 265
        Width = 360
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitWidth = 87
      end
      object ScriptTextPanel: TPanel
        Left = 1
        Top = 1
        Width = 360
        Height = 264
        Align = alTop
        Color = clOlive
        ParentBackground = False
        TabOrder = 0
        object RichEditScriptText: TRichEdit
          Left = 16
          Top = 15
          Width = 329
          Height = 234
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
      object ScriptOutputPanel: TPanel
        Left = 1
        Top = 268
        Width = 360
        Height = 84
        Align = alClient
        Color = clOlive
        ParentBackground = False
        TabOrder = 1
        object RichEditScriptOutput: TRichEdit
          Left = 16
          Top = 20
          Width = 329
          Height = 45
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
    end
  end
  object MainMenu: TMainMenu
    Left = 720
    Top = 88
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
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 655
    Top = 89
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 591
    Top = 89
  end
end
