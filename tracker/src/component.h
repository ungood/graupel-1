#ifndef COMPONENT_H
#define COMPONENT_H

template <class TContext>
class Component {
    virtual setup(TContext& context) = 0;
    virtual loop(TContext& context) = 0;
}

#endif
