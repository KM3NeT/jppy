#ifndef __JLANG__JFILEDESCRIPTORMASK__
#define __JLANG__JFILEDESCRIPTORMASK__

#include <sys/select.h>

#include "JLang/JAbstractFile.hh"
#include "JLang/JTimeval.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary class for method select.
   * This class encapsulates the <tt>fd_set</tT> data structure.
   */
  class JFileDescriptorMask :
    protected fd_set
  {
  public:


    static const int MAXIMUM_FILE_DESCRIPTOR = FD_SETSIZE;


    /**
     * Default constructor.
     */
    JFileDescriptorMask() :
      maximum_file_descriptor   (0),
      number_of_file_descriptors(0)
    {
      FD_ZERO(get());
    }


    /**
     * Constructor.
     *
     * \param  file                 file descriptor
     */
    JFileDescriptorMask(const JAbstractFile& file) :
      maximum_file_descriptor   (0),
      number_of_file_descriptors(0)
    {
      FD_ZERO(get());

      set(file);
    }


    /**
     * Constructor.
     *
     * \param  file_descriptor      file descriptor
     */
    JFileDescriptorMask(const int file_descriptor) :
      maximum_file_descriptor   (0),
      number_of_file_descriptors(0)
    {
      FD_ZERO(get());

      set(file_descriptor);
    }


    /**
     * Get pointer to mask.
     *
     * \return                      pointer to mask
     */
    const fd_set* get() const
    {
      return static_cast<const fd_set*>(this);
    }


    /**
     * Get pointer to mask.
     *
     * \return                      pointer to mask
     */
    fd_set* get()
    {
      return static_cast<fd_set*>(this);
    }


    /**
     * Address of operator.
     *
     * \return                      pointer to mask
     */
    const fd_set* operator &() const
    {
      return get();
    }


    /**
     * Address of operator.
     *
     * \return                      pointer to mask
     */
    fd_set* operator &()
    {
      return get();
    }


    /**
     * Reset mask.
     * A hard reset causes the reset of the complete mask whereas
     * a soft reset causes the reset of the internal parameters only.
     *
     * \param  option               true hard reset; else soft reset
     */
    void reset(const bool option = true)
    {
      if (option) {

	switch (number_of_file_descriptors) {

	case 0:
	  break;
	  
	case 1:
	  FD_CLR(maximum_file_descriptor, get());
	  break;
	  
	default:
	  FD_ZERO(get());
	  break;
	}
      }

      maximum_file_descriptor    = 0;
      number_of_file_descriptors = 0;
    }


    /**
     * Set file descriptor.
     *
     * \param  file_descriptor      file descriptor
     */
    void set(const int file_descriptor)
    {
      if (!has(file_descriptor)) {

	FD_SET(file_descriptor, get());

	if (file_descriptor > maximum_file_descriptor) {
	  maximum_file_descriptor = file_descriptor;
	}

	++number_of_file_descriptors;
      }
    }


    /**
     * Set file.
     *
     * \param  file                 file
     */
    void set(const JAbstractFile& file)
    {
      set(file.getFileDescriptor());
    }


    /**
     * Reset file descriptor.
     *
     * \param  file_descriptor      file descriptor
     */
    void reset(const int file_descriptor)
    {
      if (has(file_descriptor)) {

	FD_CLR(file_descriptor, get());

	while (!has(maximum_file_descriptor) && maximum_file_descriptor != 0)
	  --maximum_file_descriptor;
	
	--number_of_file_descriptors;
      }
    }


    /**
     * Reset file.
     *
     * \param  file                 file
     */
    void reset(const JAbstractFile& file)
    {
      reset(file.getFileDescriptor());
    }


    /**
     * Has file descriptor.
     *
     * \param  file_descriptor      file descriptor
     * \return                      true if file descriptor set; else false
     */
    bool has(const int file_descriptor) const
    {
      return FD_ISSET(file_descriptor, get());
    }


    /**
     * Has file.
     *
     * \param  file                 file
     * \return                      true if file set; else false
     */
    bool has(const JAbstractFile& file) const
    {
      return has(file.getFileDescriptor());
    }


    /**
     * Get number of file descriptors.
     *
     * \return                      number of file descriptors
     */
    int getNumberOfFileDescriptors() const
    {
      return maximum_file_descriptor;
    }


    /**
     * Check setting of file descriptors.
     *
     * \return                      true if no file descriptors are set; else false
     */
    bool empty() const
    {
      return number_of_file_descriptors == 0;
    }


    /**
     * Check availability of input.
     * This method returns true is at least one byte can be read without blocking.
     * Following a select() call, this method overwrites the internal mask!
     *
     * \param  timeout      timeout
     * \return              true if ready to read; else false
     */
    bool in_avail(JTimeval timeout = JTimeval::min())
    {
      return ::select(getNumberOfFileDescriptors() + 1, get(), NULL, NULL, &timeout) > 0;
    }


    /**
     * Check availability of output.
     * This method returns true is at least one byte can be written without blocking.
     * Following a select() call, this method overwrites the internal mask!
     *
     * \param  timeout      timeout
     * \return              true if ready to write; else false
     */
    bool out_avail(JTimeval timeout = JTimeval::min())
    {
      return ::select(getNumberOfFileDescriptors() + 1, NULL, get(), NULL, &timeout) > 0;
    }


  private:
    int maximum_file_descriptor;
    int number_of_file_descriptors;
  };
}

#endif
