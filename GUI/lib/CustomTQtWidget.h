#ifndef CUSTOMTQTWIDGET_H
#define CUSTOMTQTWIDGET_H

#include <TQtWidget.h>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>

/** \Class TreeBuilder 
 * 
 * We need to override the TQtWidget class in order to implement the selection functionality
 * The whole purpose of this subclass is to take over the mouse events when the need arises
 *
 */ 
class CustomTQtWidget : public TQtWidget {

    Q_OBJECT
 
    public:
        CustomTQtWidget(QWidget *parent = 0);
        ~CustomTQtWidget();

        /**
         * return whether the y-axis is required to be "locked" -- 
         * this is done in the case of 1D plots where we only want to scan the x-axis 
         * to make a selection
         */
        bool getLockY();
        /**
         * return whether the the mouse events have been "taken over" for selection purposes
         */
        bool getTakeoverMouse();

        /**
         * set the flag to lock the y-axis
         */
        void setLockY(bool);
        /**
         * set the flag to take over the mouse events
         */
        void setTakeoverMouse(bool);

    signals:
        /**
         * signal emitted after a selection is made. Provides information about the selection coordinates
         */
        void selectSignal(QPoint, QPoint);
        /**
         * signal emitted after a double click. Meant to zoom out 
         */
        void zoomoutSignal();

    protected:
        /**
         * set the point of origin of the selection
         */
        virtual void mousePressEvent(QMouseEvent*);
        /**
         * update the selection rectangle as the mouse moves
         */
        virtual void mouseMoveEvent(QMouseEvent*);
        /**
         * get the end-point of the selection and emit the selection signal
         */
        virtual void mouseReleaseEvent(QMouseEvent*);
        /**
         * Unzoom the plot
         */
        virtual void mouseDoubleClickEvent(QMouseEvent*);

    private:
        /**
         * The rectangle depicting the selection area while the mouse is moving
         */
        QRubberBand rubberBand;
        /**
         * Starting point of the selection
         */
        QPoint origin;
        /**
         * End point of the selection
         */
        QPoint endpoint;
        /**
         * Flag to lock the y-axis
         */
        bool lockY;
        /**
         * Flag to take over the mouse from the TQtWidget
         */
        bool takeoverMouse;
};

#endif
