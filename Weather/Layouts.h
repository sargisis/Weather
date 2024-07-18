#ifndef LAYOUTS_H
#define LAYOUTS_H

class Layouts
{
public:
     Layouts() = default;
protected:
     virtual void createLayouts() = 0;
protected:
     virtual ~Layouts() {}

};

#endif // LAYOUTS_H
