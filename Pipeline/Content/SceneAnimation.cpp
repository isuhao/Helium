#include "Scene.h"

#include "Pipeline/Content/Animation/Animation.h"
#include "Pipeline/Content/Animation/AnimationClip.h"

namespace Content
{
  u32 Scene::GetNumAnimationClips() const
  {
    return (u32)(m_AnimationClips.size());
  }

  u32 Scene::GetNumValidJointAnimations( u32 clipIndex ) const
  {
    return GetNumValidJointAnimations( m_JointIds, clipIndex );
  }

  u32 Scene::GetNumValidJointAnimations( const Nocturnal::S_TUID& jointList, u32 clipIndex ) const
  {
    u32 validJointAnimations = 0;

    for each ( Nocturnal::TUID jointId in jointList )
    {
      if ( m_AnimationClips[ clipIndex ]->m_JointAnimationMap.find( jointId ) != m_AnimationClips[ clipIndex ]->m_JointAnimationMap.end() )
        ++validJointAnimations;
    }

    return validJointAnimations;
  }

  void Scene::GetJointMismatchReport( std::vector< tstring > &mismatchMessages, u32 clipIndex ) const
  {
    std::set< Nocturnal::TUID > animatedJoints;

    M_Animation::iterator itr = m_AnimationClips[ clipIndex ]->m_JointAnimationMap.begin();
    M_Animation::iterator end = m_AnimationClips[ clipIndex ]->m_JointAnimationMap.end();
    
    tstring jointIdAsString;
    for( ; itr != end; ++itr )
    {
      if ( m_JointIds.find( (*itr).first ) == m_JointIds.end() )
      {
        (*itr).first.ToString( jointIdAsString );
        mismatchMessages.push_back( tstring( TXT( "joint id [" ) ) + jointIdAsString + TXT( "] not found in scene!" ) );
        continue;
      }

      if ( (*itr).second->WindowSamples() == 0 )
      {
       (*itr).first.ToString( jointIdAsString );
        mismatchMessages.push_back( tstring( TXT( "joint id [" ) ) + jointIdAsString + TXT( "] has no samples!" ) );
      }

      animatedJoints.insert( (*itr).first );
    }

    for each( Nocturnal::TUID jointId in m_JointIds )
    {
      if ( animatedJoints.find( jointId ) == animatedJoints.end() )
      {
        jointId.ToString( jointIdAsString );
        mismatchMessages.push_back( tstring( TXT( "joint id [" ) ) + jointIdAsString + TXT( "] was not animated!" ) );
      }
    }
  }
}