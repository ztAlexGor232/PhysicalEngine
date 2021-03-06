#include "drawmanager.h"

DrawManager::DrawManager(){}

QGraphicsItem* DrawManager::getPainter(Body* b){
    Shape* s = b->getShape();
    QGraphicsItem* drawnShape;
    switch (s->getType()){
    case s->CIRCLE: {
         drawnShape = circlePainterBuilder(b);
         break;
    }
    case s->RECT: {
        drawnShape = rectPainterBuilder(b);
        break;
    }
    }
    return drawnShape;
}

QGraphicsItem* DrawManager::getPainter(EngineObject* obj){
    Shape* s = obj->getBody()->getShape();
    QGraphicsItem* drawnShape;
    switch (s->getType()){
    case s->CIRCLE: {
         drawnShape = circlePainterBuilder(obj);
         break;
    }
    case s->RECT: {
        drawnShape = rectPainterBuilder(obj);
        break;
    }
    }
    return drawnShape;
}

QGraphicsItem* DrawManager::circlePainterBuilder(Body* b){
    Circle *c  = reinterpret_cast<Circle *>(b->getShape());

    return new CirclePainter(b->getPos(), c->getRadius(), new QPen(Qt::black, 4));
}

QGraphicsItem* DrawManager::rectPainterBuilder(Body* b){
    Rectangle *r  = reinterpret_cast<Rectangle *>(b->getShape());
    QPointF p = b->getPos();

    return new RectPainter(p, r->getWidth(), r->getHeight(), b->getAngle(), new QPen(Qt::black, 4));
}

QGraphicsItem* DrawManager::circlePainterBuilder(EngineObject* obj){
    Circle *c  = reinterpret_cast<Circle *>(obj->getBody()->getShape());
    QPen* pen = new QPen(*obj->getPen());
    if (obj->isSelected()){
        pen->setColor(Qt::blue);
        obj->setSelect(false);
    }
    return new CirclePainter(obj->getBody()->getPos(), c->getRadius(), pen);
}

QGraphicsItem* DrawManager::rectPainterBuilder(EngineObject* obj){
    Rectangle *r  = reinterpret_cast<Rectangle *>(obj->getBody()->getShape());
    QPointF p = obj->getBody()->getPos();
    QPen* pen = new QPen(*obj->getPen());
    if (obj->isSelected()){
        pen->setColor(Qt::blue);
        obj->setSelect(false);
    }
    return new RectPainter(p, r->getWidth(), r->getHeight(), obj->getBody()->getAngle(), pen);
}


//-----------------------------------------------------------------------------------------------------------//

CirclePainter::CirclePainter(QPointF center, float radius, QPen* pen): center(center), radius(radius), pen(pen){
    this->thickness = pen->width();
}

//CirclePainter::CirclePainter(QPointF center, float radius, float t): center(center), radius(radius), thickness(t){}

void CirclePainter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(*pen);

    float d = radius - thickness/2;
    painter->drawEllipse(center.x() - d, center.y() - d, 2*d, 2*d);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF CirclePainter::boundingRect() const{
    float d = radius + thickness/2;
    //return QRectF(QPoint(center.x() - d, center.y() - d),
    //              QPoint(center.x() + d, center.y() + d));
    return QRectF(QPointF(0, 0), QPointF(1650, 1000));
}


//-----------------------------------------------------------------------------------------------------------//

RectPainter::RectPainter(QPointF v, float w, float h, float a, QPen* pen): v(v), w(w), h(h), a(a), pen(pen){
    this->thickness = pen->width();
}

//RectPainter::RectPainter(QPointF v, float w, float h, float a, float t): v(v), w(w), h(h), a(a), thickness(t){}

void RectPainter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(*pen);

    painter->translate(v.x(), v.y());
    painter->rotate(a * 180 / 3.1415);//
    //painter->drawRect(thickness / 2, thickness / 2,
    //                  w - thickness , h - thickness);

    painter->drawRect(thickness / 2 - w/2, thickness / 2 - h/2,
                      w - thickness , h - thickness);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF RectPainter::boundingRect() const{
    return QRectF(QPointF(0, 0), QPointF(1650, 1000));
    //return QRectF(QPoint(v.x() - thickness / 2, v.y() - thickness / 2),
    //              QPoint(v.x() + w + thickness / 2, v.y() + h + thickness / 2));
}
