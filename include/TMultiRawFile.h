#ifndef _TMULTIRAWFILE_H_
#define _TMULTIRAWFILE_H_

#include <set>
#ifndef __CINT__
#   include <mutex>
#endif

#include "TObject.h"

#include "TRawEvent.h"
#include "TRawEventSource.h"

struct FileEvent {
  TRawEventSource* file;
  TRawEvent next_event;

  // Needed for keeping the std::set sorted by timestamp
  bool operator<(const FileEvent& other) const;
};

bool file_event_sorting(const FileEvent& a, const FileEvent& b);

class TMultiRawFile : public TRawEventSource {
public:
  TMultiRawFile();
  ~TMultiRawFile();
  // TODO, can I disable these somehow without annoying ROOT?
  TMultiRawFile(const TMultiRawFile& other) { }
  TMultiRawFile& operator=(const TMultiRawFile& other) { }

  void AddFile(TRawEventSource* infile);
  void AddFile(const char* filename);

  bool IsValid() const { return fIsValid; }

  virtual std::string SourceDescription() const;
  virtual std::string Status() const;
  virtual int GetLastErrno() const;
  virtual std::string GetLastError() const;

private:
  std::set<TRawEventSource*> fFileList; // This list does not get modified frequently
  std::set<FileEvent> fFileEvents; // This list is modified frequently

#ifndef __CINT__
  mutable std::mutex fFileListMutex;
#endif

  virtual int GetEvent(TRawEvent& outevent);
  mutable bool fIsFirstStatus;
  bool fIsValid;


  ClassDef(TMultiRawFile, 0);
};




#endif
