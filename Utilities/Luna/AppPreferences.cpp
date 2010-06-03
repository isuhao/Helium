#include "Precompile.h"
#include "AppPreferences.h"

#include "AppUtils/Preferences.h"

using namespace Luna;

REFLECT_DEFINE_CLASS( AppPreferences );

void AppPreferences::EnumerateClass( Reflect::Compositor< AppPreferences >& comp )
{
  Reflect::Field* fieldUseTracker = comp.AddField( &AppPreferences::m_UseTracker, "m_UseTracker" );
}

AppPreferencesPtr g_AppPreferences = NULL;

// Increment this value to invalidate all previously saved preferences
const static std::string s_AppPreferencesVersion( "1" );

///////////////////////////////////////////////////////////////////////////////
// Static initialization.
// 
void AppPreferences::InitializeType()
{
  Reflect::RegisterClass< AppPreferences >( "AppPreferences" );

  g_AppPreferences = new AppPreferences();
  g_AppPreferences->LoadPreferences();
}

///////////////////////////////////////////////////////////////////////////////
// Static cleanup.
// 
void AppPreferences::CleanupType()
{
  g_AppPreferences = NULL;
  Reflect::UnregisterClass< AppPreferences >();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the global preferences.  You must call AppPreferences::InitializeType first.
// 
AppPreferences* Luna::GetAppPreferences()
{
  if ( !g_AppPreferences.ReferencesObject() )
  {
    throw Nocturnal::Exception( "AppPreferences is not initialized, must call AppPreferences::InitializeType first." );
  }

  return g_AppPreferences;
}

///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
AppPreferences::AppPreferences()
: m_UseTracker( true )
{
}

///////////////////////////////////////////////////////////////////////////////
// Returns the current version of the preferences.  Changing this value 
// invalidates all previously saved preferences.  You can also invalidate
// separate aspects of the preferences.  See the globals section at the top
// of this file.
// 
const std::string& AppPreferences::GetCurrentVersion() const 
{
  return s_AppPreferencesVersion;
}

///////////////////////////////////////////////////////////////////////////////
// Load preferences.
// 
std::string AppPreferences::GetPreferencesPath() const
{
    Nocturnal::Path prefsPath;
    if ( !AppUtils::GetPreferencesDirectory( prefsPath ) )
    {
        throw Nocturnal::Exception( "Could not get preferences directory." );
    }

    prefsPath.Set( prefsPath.Get() + "/Luna/AppPreferences.rb" );
    return prefsPath.Get();
}

///////////////////////////////////////////////////////////////////////////////
// Sets the tracker setting and raises an event if it changed.
// 
void AppPreferences::UseTracker( bool useTracker )
{
  if ( m_UseTracker != useTracker )
  {
    m_UseTracker = useTracker;
    RaiseChanged( UseTrackerField() );
  }
}

///////////////////////////////////////////////////////////////////////////////
// Gets the tracker field
// 
const Reflect::Field* AppPreferences::UseTrackerField() const
{
  return GetClass()->FindField( &AppPreferences::m_UseTracker );
}
