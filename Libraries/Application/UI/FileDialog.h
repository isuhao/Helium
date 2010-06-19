#pragma once

#include "Application/API.h"

#include <wx/filedlg.h>

#include "Platform/Compiler.h"
#include "Platform/Types.h"
#include "Foundation/Container/OrderedSet.h"

///////////////////////////////////////////////////////////////////////////

namespace Nocturnal
{
#define WXFD_STYLEOFFSET 16

  namespace FileDialogStyles
  {
    // bit flags
    enum FileDialogStyle
    {
      Open                  = wxFD_OPEN,              // Open; to open one or more assets
      Save                  = wxFD_SAVE,              // Save; use to save files (should pass in default fullPath or location)

      OverwritePrompt       = wxFD_OVERWRITE_PROMPT,  // prompts if the file exists;        dialogs: Save
      FileMustExist         = wxFD_FILE_MUST_EXIST,   // file must exist on disk;           dialogs: Open
      Multiple              = wxFD_MULTIPLE,          // select several files at once;      dialogs: Open
      ChangeCWD             = wxFD_CHANGE_DIR,        // change the current working dir;    dialogs: All

      CreateFolder          = 1 << ( WXFD_STYLEOFFSET + 1 ),   // allow the user to make new folder; dialogs: Save
      ExportFile            = 1 << ( WXFD_STYLEOFFSET + 2 ),   // let's them save file anywhere      dialogs: Save

      ShowAllFilesFilter    = 1 << ( WXFD_STYLEOFFSET + 3 ),   // show "All files (*.*) option;      dialogs: All
    };

    // Masks
    static const i32 DefaultOpen   = ( Open | FileMustExist );
    static const i32 DefaultSave   = ( Save | CreateFolder | OverwritePrompt );

  }
  typedef i32 FileDialogStyle;


  /////////////////////////////////////////////////////////////////////////////

  typedef Nocturnal::OrderedSet< std::string > OS_string;

  class APPLICATION_API FileDialog: public wxFileDialog
  {

  private:

    FileDialogStyle         m_Style;
    OS_string           m_Filters;

  protected:

    S_string                m_Files;


  public:

    //
    // Ctor/Dtor
    //
    FileDialog( wxWindow *parent,
      const wxString& message = wxFileSelectorPromptStr,
      const wxString& defaultDir = wxEmptyString,
      const wxString& defaultFile = wxEmptyString,
      const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
      FileDialogStyle style = FileDialogStyles::DefaultOpen,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& sz = wxDefaultSize,
      const wxString& name = wxFileDialogNameStr );

    virtual ~FileDialog();
    virtual int ShowModal();
    bool IsMultipleSelectionEnabled() const;

    virtual wxString GetPath() const NOC_OVERRIDE;
    virtual void GetPaths( wxArrayString& paths ) const NOC_OVERRIDE;
    virtual const std::string& GetFilePath() const;
    virtual const S_string& GetFilePaths() const;

    void SetFilter( const std::string& filter );
    void SetFilterIndex( const std::string& filter );
    void AddFilter( const std::string& filter );

  protected:
    void OnOkButtonClicked( wxCommandEvent& evt );

  protected:

    DECLARE_EVENT_TABLE()

  private:

    virtual bool Create( wxWindow *parent,
      const wxString& message = wxFileSelectorPromptStr,
      const wxString& defaultDir = wxEmptyString,
      const wxString& defaultFile = wxEmptyString,
      const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
      long style = wxFD_DEFAULT_STYLE,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& sz = wxDefaultSize,
      const wxString& name = wxFileDialogNameStr );

    void UpdateFilter();
  };

} // namespace Nocturnal