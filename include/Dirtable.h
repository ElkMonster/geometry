#ifndef DIRTABLE_H_
#define DIRTABLE_H_


#define CLEAN_IF_DIRTY(pdirtable)                   \
    if ((pdirtable)->dirty_) (pdirtable)->makeClean()

// Generates a getter that calls makeClean() before returning.
// Use like this:
// MAKE_GETTER(int foo::getVal(), some_var)
#define MAKE_GETTER(func_sig, ret_val)  \
    func_sig                            \
    {                                   \
        CLEAN_IF_DIRTY(this);           \
        return ret_val;                 \
    }

// Generates a setter that sets the dirty flag.
// Use like this:
// MAKE_SETTER(foo::setVal(int i), myInt, i)
// Note that void return type is already specified in the macro!
#define MAKE_SETTER(func_name, rec, val)    \
    void func_name                          \
    {                                       \
        rec = val;                          \
        dirty_ = true;                      \
    }


class Dirtable
{

public:

    Dirtable();

    Dirtable(bool state);

    virtual ~Dirtable();

protected:

    virtual void performCleaning() const = 0;

    void makeClean() const;

protected:

    mutable bool dirty_;

};

#endif // DIRTABLE_H_
