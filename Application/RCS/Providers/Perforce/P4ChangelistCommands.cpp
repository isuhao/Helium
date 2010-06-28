#include "P4ChangelistCommands.h"
#include "P4Tags.h"

#include "Foundation/Log.h"
#include "Platform/String.h"

using namespace Perforce;

void OpenedCommand::OutputStat( StrDict* dict )
{
  RCS::File file;

  bool converted = Platform::ConvertString( dict->GetVar( g_DepotFileTag )->Text(), file.m_DepotPath );
  NOC_ASSERT( converted );

  converted = Platform::ConvertString( dict->GetVar( g_ClientFileTag )->Text(), file.m_LocalPath );
  NOC_ASSERT( converted );

  file.m_LocalRevision = dict->GetVar( g_RevisionTag )->Atoi();
  
  tstring actionString;
  converted = Platform::ConvertString( dict->GetVar( g_ActionTag )->Text(), actionString );
  NOC_ASSERT( converted );

  file.m_Operation = GetOperationEnum( actionString );

  file.m_ChangesetId = dict->GetVar( g_ChangeTag )->Atoi();
  
  tstring fileType;
  converted = Platform::ConvertString( dict->GetVar( g_TypeTag )->Text(), fileType );
  NOC_ASSERT( converted );

  file.m_FileType = GetFileType( fileType );

  converted = Platform::ConvertString( dict->GetVar( g_UserTag )->Text(), file.m_Username );
  NOC_ASSERT( converted );

  converted = Platform::ConvertString( dict->GetVar( g_ClientTag )->Text(), file.m_Client );
  NOC_ASSERT( converted );

  m_FileList->push_back( file );
}

void SubmitCommand::OutputStat( StrDict* dict )
{
  if ( m_Changeset == NULL )
  {
    return;
  }

  StrPtr* submittedChange = dict->GetVar( g_SubmittedChangeTag );
  if ( submittedChange )
  {
    m_Changeset->m_Id = submittedChange->Atoi();
  }

  u32 numFiles = 0;
  StrPtr* openFiles = dict->GetVar( g_OpenFilesTag );
  if ( openFiles )
  {
    numFiles = openFiles->Atoi();
  }

  for( u32 i = 0; i < numFiles; ++i )
  {
    RCS::FilePtr file = new RCS::File();

    StrPtr* depotFile = dict->GetVar( g_DepotFileTag, i );
    if ( depotFile )
    {
        bool converted = Platform::ConvertString( depotFile->Text(), file->m_DepotPath );
        NOC_ASSERT( converted );
    }

    StrPtr* revision = dict->GetVar( g_RevisionTag );
    if ( revision )
    {
      file->m_LocalRevision = revision->Atoi();
    }

    StrPtr* action = dict->GetVar( g_ActionTag );
    if ( action )
    {
        tstring actionString;
        bool converted = Platform::ConvertString( action->Text(), actionString );
        NOC_ASSERT( converted );

        file->m_Operation = GetOperationEnum( actionString );
    }
  }
}

void RevertCommand::OutputStat( StrDict* dict )
{
  if ( m_File == NULL )
  {
    return;
  }

  bool converted = Platform::ConvertString( dict->GetVar( g_DepotFileTag )->Text(), m_File->m_DepotPath );
  NOC_ASSERT( converted );

  converted = Platform::ConvertString( dict->GetVar( g_ClientFileTag )->Text(), m_File->m_LocalPath );
  NOC_ASSERT( converted );

  m_File->m_LocalRevision = dict->GetVar( g_HaveRevTag )->Atoi();
  
  tstring actionString;
  converted = Platform::ConvertString( dict->GetVar( g_OldActionTag )->Text(), actionString );
  NOC_ASSERT( converted );

  m_File->m_Operation = GetOperationEnum( actionString );
}

void CreateChangelistCommand::InputData( StrBuf *buf, Error *e )
{
  tstring spec;
  spec  = TXT( "Change: new\n\n" );
  spec += TXT( "Client: " );
  spec += m_Provider->m_ClientName + TXT( "\n\n" );
  spec += TXT( "User: " );
  spec += m_Provider->m_UserName + TXT( "\n\n" );
  spec += TXT( "Description:\n\t" );
  spec += m_Changeset->m_Description;

  std::string narrowSpec;
  bool converted = Platform::ConvertString( spec, narrowSpec );
  NOC_ASSERT( converted );
  buf->Set( narrowSpec.c_str() );
}

void CreateChangelistCommand::Run()
{
  AddArg( TXT( "-i" ) );
  Command::Run();
}

// gah, the change command doesn't seem to have stat output... ?
void CreateChangelistCommand::OutputInfo( char level, const char* data )
{
  if ( sscanf_s( data, "Change %d", &m_Changeset->m_Id ) != 1 )
  {
    Log::Error( TXT( "Could not parse perforce changelist creation message:\n\n %s\n\nPlease contact support with this error message.\n" ) );
    m_ErrorCount++;
  }
}

void GetChangelistsCommand::Run()
{
  AddArg( TXT( "-c" ) );
  AddArg( m_Provider->m_ClientName );
  AddArg( TXT( "-s" ) );
  AddArg( TXT( "pending" ) );

  Command::Run();
}

void GetChangelistsCommand::OutputStat( StrDict* dict )
{
  RCS::Changeset changeset;

  changeset.m_Id = dict->GetVar( g_ChangeTag )->Atoi();
  
  bool converted = Platform::ConvertString( dict->GetVar( g_DescriptionTag )->Text(), changeset.m_Description );
  NOC_ASSERT( converted );

  m_Changesets->push_back( changeset );
}