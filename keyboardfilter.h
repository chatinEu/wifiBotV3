#ifndef KEYBOARDFILTER_H
#define KEYBOARDFILTER_H

#include <QObject>

class KeyBoardFilter : public QObject
{
    Q_OBJECT
public:
    explicit KeyBoardFilter(QObject *parent = nullptr);


signals:
  void up_arrow();
  void down_arrow();
  void left_arrow();
  void right_arrow();
protected:
  bool eventFilter( QObject *dist, QEvent *event );
};

#endif // KEYBOARDFILTER_H
