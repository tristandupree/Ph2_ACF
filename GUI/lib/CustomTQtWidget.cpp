#include "CustomTQtWidget.h"

#include <QRect>
#include <TH1F.h>

#include<iostream>

CustomTQtWidget::CustomTQtWidget(QWidget *): 
    rubberBand(QRubberBand::Rectangle, this),
    origin(0, 0),
    endpoint(0, 0),
    lockY(true),
    takeoverMouse(false)
{
}

CustomTQtWidget::~CustomTQtWidget() {
}

void CustomTQtWidget::mousePressEvent(QMouseEvent* event) {
    // If the mouse is not flagged for take-over simply do what the parent TQtWidget class does
    if (!takeoverMouse) {
        TQtWidget::mousePressEvent(event);
    }
    // Determine the pixel coordinates of the starting point of the selection. 
    else {
        origin = event->pos();
        // Constrain the selection area to be within the plot area
        if (origin.x() > GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax())) origin.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax()));
        if (origin.x() < GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin())) origin.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin()));
        if (origin.y() < GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax())) origin.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax()));
        if (origin.y() > GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin())) origin.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin()));
        // lock the y coordinate of the starting point
        if (lockY) origin.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin()));

        rubberBand.setGeometry(QRect(origin, QSize()));
        rubberBand.show();
    }
}

void CustomTQtWidget::mouseMoveEvent(QMouseEvent* event) {

    if (!takeoverMouse) {
        TQtWidget::mouseMoveEvent(event);
    }

    else {
        QPoint pt = event->pos();
        // Constrain the selection area to be within the plot area
        if (pt.x() > GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax())) pt.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax()));
        if (pt.x() < GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin())) pt.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin()));
        if (pt.y() < GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax())) pt.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax()));
        if (pt.y() > GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin())) pt.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin()));
        // lock the y coordinate starting point of the 
        if (lockY) pt.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax()));

        rubberBand.setGeometry(QRect(origin, pt).normalized());
    }
}

void CustomTQtWidget::mouseReleaseEvent(QMouseEvent* event) {

    if (!takeoverMouse) {
        TQtWidget::mouseReleaseEvent(event);
    }

    // Determine the pixel coordinates of the end-point of the selection. 
    else {
        endpoint = event->pos();
        // Constrain the selection area to be within the plot area
        if (endpoint.x() > GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax())) endpoint.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmax()));
        if (endpoint.x() < GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin())) endpoint.setX(GetCanvas()->XtoAbsPixel(GetCanvas()->GetUxmin()));
        if (endpoint.y() < GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax())) endpoint.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax()));
        if (endpoint.y() > GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin())) endpoint.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymin()));
        // lock the y coordinate of the end point
        if (lockY) endpoint.setY(GetCanvas()->YtoAbsPixel(GetCanvas()->GetUymax()));

        // make the selection rectangle disapper after the mouse is released
        rubberBand.hide();
        // emit the signal informing about the selection
        emit selectSignal(origin, endpoint);
    }
}

void CustomTQtWidget::mouseDoubleClickEvent(QMouseEvent*) {
    emit zoomoutSignal();
}

bool CustomTQtWidget::getLockY() {
    return lockY;
}

bool CustomTQtWidget::getTakeoverMouse() {
    return takeoverMouse;
}

void CustomTQtWidget::setLockY(bool l) {
    lockY = l;
}

void CustomTQtWidget::setTakeoverMouse(bool t) {
    takeoverMouse = t;
}
