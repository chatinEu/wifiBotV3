#include "keyboardfilter.h"
#include "QEvent"
#include"QKeyEvent"
#include "iostream"

KeyBoardFilter::KeyBoardFilter(QObject *parent) : QObject(parent)
{}

bool KeyBoardFilter::eventFilter(QObject *dist, QEvent *event)
{
    if( event->type() == QEvent::KeyPress )
        {

        std::cout<<"this is a key evnet ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;";
          QKeyEvent *keyEvent = static_cast<QKeyEvent*>( event );
          if ( keyEvent->key() == Qt::Key_Up ) {emit up_arrow(); return true;}
          else if ( keyEvent->key() == Qt::Key_Down ) {emit down_arrow(); return true;}
          else if ( keyEvent->key() == Qt::Key_Left ) {emit left_arrow(); return true;}
          else if ( keyEvent->key() == Qt::Key_Right ) {emit right_arrow(); return true;}
        }
    return false;
}
