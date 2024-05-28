// smart pointer by Phaton

/*! 
  \internal
  \file
  \brief Smart pointers with reference counter.
*/

#ifndef H_REFERENCE_COUNTING
#define H_REFERENCE_COUNTING

/*!
  \internal
  \brief Reference counter.
*/

class RefCntObject
{
public:
  virtual ~RefCntObject() {}

  /** Default constructor. Initial reference count is 0,
      and will be incremented as soon as the object is
      pointed to. */
  RefCntObject() : m_refcnt(0) {}

  /** Add 1 to the reference count. */
  void addRef() { m_refcnt++; }

  /** Subtract 1 from the reference count.
      Returns true if the reference count has reached 0
      and the object should be deleted. */
  bool subRef() { return (--m_refcnt <= 0); }

private:
  int m_refcnt;
};

/*!
  \internal
  \brief Smart pointer with reference counter.
*/
template<class T>
class RefCntPointer
{
public:
  /** Construct from normal pointer, default to NULL */
  RefCntPointer(T* ptr=0) : m_ptr(ptr) { addRef(); }
  
  /** Construct from another smart pointer.  Copy Constructor */
  RefCntPointer(const RefCntPointer& p) : m_ptr(p.m_ptr) { addRef(); }

  /** Destructor. */
  ~RefCntPointer() { subRef(); }

  /** Assignment operator. */
  RefCntPointer& operator= (const RefCntPointer& p)
  {
    return *this = p.m_ptr;
  }

  /** Assignment operator. */
  RefCntPointer& operator= (T* ptr)
  {
    if (m_ptr != ptr)
    {
      subRef();
      m_ptr=ptr;
      addRef();
    }
    return *this;
  }

  /** Dereferencing operator. Provided to behave like the normal pointer. */
  T& operator *        () const { return *m_ptr; }

  /** Member access operator. Provided to behave like the normal pointer. */
  T* operator ->       () const { return m_ptr; }

  // Conversion operators
     operator T*       () const { return m_ptr; }
     operator const T* () const { return m_ptr; }

  /** boolean test for NULL */
	 operator bool     () const { return m_ptr!=0; }

  /** Address of pointer.  May cause memory leaks if pointer is modified. */
  T** operator &       ()       { return &m_ptr; }

private:
  void addRef()
  {
    // Only change if non-null
    if (m_ptr) m_ptr->addRef();
  }

  void subRef()
  {
    // Only change if non-null
    if (m_ptr)
    {
      // Subtract and test if this was the last pointer.
	  if (m_ptr->subRef())
      {
        delete m_ptr;
        m_ptr=0;
      }
    }
  }

  T* m_ptr;
};

#endif