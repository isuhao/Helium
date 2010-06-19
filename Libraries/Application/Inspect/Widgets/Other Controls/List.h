#pragma once

#include "Application/API.h"
#include "Application/Inspect/Widgets/Control.h"

#include "Foundation/Reflect/Class.h"

namespace Inspect
{
  //
  // Listbox control
  //

  const static char LIST_ATTR_SORTED[]    = "sorted";
  
  namespace MoveDirections
  {
    enum MoveDirection
    {
      Up,
      Down
    };
  }
  typedef MoveDirections::MoveDirection MoveDirection;

  class APPLICATION_API List : public Reflect::ConcreteInheritor<List, Control>
  {
  public:
    // Delimiter to use for key-value pairs if this list is displaying a map of data
    static const char* s_MapKeyValDelim; 

  protected:
    V_string m_Items;
    V_string m_SelectedItems;
    bool m_Sorted;
    bool m_IsMap; // Is the data bound to this control acutally a std::map?

  public:
    List();

    virtual void Realize( Container* parent ) NOC_OVERRIDE;
    virtual void Read() NOC_OVERRIDE;
    virtual bool Write() NOC_OVERRIDE;

    void SetSorted( bool sort );

    void SetMap( bool isMap );


    const V_string& GetItems();
    void AddItems( const V_string& items );

    void AddItem( const std::string& item );
    void RemoveItem( const std::string& item );

    const V_string& GetSelectedItems();
    void SetSelectedItems( const V_string& items );

    std::string GetSelectedItems( const std::string delimiter );
    void SetSelectedItems( const std::string& delimitedList, const std::string& delimiter );

    void MoveSelectedItems( MoveDirection direction = MoveDirections::Up );

  protected:
    std::string GetDelimitedList( const V_string& items, const std::string& delimiter );
    void UpdateUI( const V_string& items );
    virtual bool Process(const std::string& key, const std::string& value) NOC_OVERRIDE;
  };

  typedef Nocturnal::SmartPtr<List> ListPtr;
}