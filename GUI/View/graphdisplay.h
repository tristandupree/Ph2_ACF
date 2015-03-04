#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H

#include <QWidget>

namespace Ui {
    class GraphDisplay;
}

class GraphDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GraphDisplay(QWidget *parent = 0);
    ~GraphDisplay();

private:
    Ui::GraphDisplay *ui;
};

#endif // GRAPHDISPLAY_H
