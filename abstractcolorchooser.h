#ifndef ABSTRACTCOLORCHOOSER_H
#define ABSTRACTCOLORCHOOSER_H

#include <QWidget>
#include <QColor>

class AbstractColorChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractColorChooser(QWidget *parent, QColor in_Color);

private:
    QColor m_color;

protected:
    virtual void paintEvent(QPaintEvent *event) = 0;
};

#endif // ABSTRACTCOLORCHOOSER_H
