#ifndef __vtkDICOMParser_h
#define __vtkDICOMParser_h

#include <vtkObject.h>

class vtkDICOMMetaData;

//! A meta data reader for DICOM data.
/*!
 *  This class provides routines for parsing a DICOM file
 *  and creating a vtkDICOMMetaData object.
 */
class vtkDICOMParser : public vtkObject
{
public:
  //! Create a new vtkDICOMParser instance.
  static vtkDICOMParser *New();

  //! VTK dynamic type information macro.
  vtkTypeMacro(vtkDICOMParser, vtkObjectBase);

  //! Print a summary of the contents of this object.
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Set the file name.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  //! Set the metadata object for storing the data elements.
  void SetMetaData(vtkDICOMMetaData *);
  vtkDICOMMetaData *GetMetaData() { return this->MetaData; }

  //! Set the instance index to use when storing metadata.
  vtkSetMacro(Index, int);
  int GetIndex() { return this->Index; }

  //! Get the byte offset to the end of the metadata.
  /*!
   *  After the metadata has been read, the file offset
   *  will be set to the position of the pixel data.
   */
  vtkTypeInt64 GetFileOffset() { return this->FileOffset; }

  //! Set the buffer size, the default is 8192 (8k).
  /*!
   *  A larger buffer size results in fewer IO calls.  The
   *  minimum buffer size is 256 bytes.
   */
  void SetBufferSize(int size);
  int GetBufferSize() { return this->BufferSize; }

  //! Read the metadata from the file.
  void Update() { this->ReadFile(this->MetaData, this->Index); }

  //! Internal method for filling the buffer, do not use.
  /*!
   *  This is an internal method that refills the buffer
   *  by reading data from the file.  The pointers cp and
   *  ep mark the region of the buffer that has not been
   *  parsed yet.  The call to FillBuffer will move this
   *  region to the beginning of the buffer, and will then
   *  fill the remainder of the buffer with new data from
   *  the file.  The values of cp and ep will be set to the
   *  beginning and end of the buffer.
   */
  virtual bool FillBuffer(
    const unsigned char* &cp, const unsigned char* &ep);

protected:
  vtkDICOMParser();
  ~vtkDICOMParser();

  //! Read the file into the provided metadata object.
  virtual bool ReadFile(vtkDICOMMetaData *data, int idx);

  //! Read just the meta header (group 0x0002).
  bool ReadMetaHeader(
    const unsigned char* &cp, const unsigned char* &ep,
    vtkDICOMMetaData *data, int idx);

  //! Read the meta data following the meta header.
  bool ReadMetaData(
    const unsigned char* &cp, const unsigned char* &ep,
    vtkDICOMMetaData *data, int idx);

  //! Compute the file offset to the current position.
  void ComputeFileOffset(
    const unsigned char* cp, const unsigned char* ep);

  char *FileName;
  vtkDICOMMetaData *MetaData;
  std::istream *InputStream;
  std::streamsize BytesRead;
  vtkTypeInt64 FileOffset;
  char *Buffer;
  int BufferSize;
  int ChunkSize;
  int Index;

private:
  vtkDICOMParser(const vtkDICOMParser&);  // Not implemented.
  void operator=(const vtkDICOMParser&);  // Not implemented.
};

#endif /* __vtkDICOMParser_h */