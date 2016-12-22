#ifndef SOURCE_COMPONENTS_DATASOURCES_EPICSDATASOURCE_SIGBLOCKDOUBLEBUFFER_SNIPPET_H_
#define SOURCE_COMPONENTS_DATASOURCES_EPICSDATASOURCE_SIGBLOCKDOUBLEBUFFER_SNIPPET_H_

struct SigblockDoubleBuffer_v1 {
	static const unsigned int TWO = 2;
	enum BufferStatus {FREE, FULL, WRITING, READING};
    std::size_t sizeOfSigblock;
    unsigned int status[2]; //The status could be placed at the beginning of buffers, too.
    char buffer[]; //Array of 2 Sigblocks
    void Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock);
    bool Get(Sigblock& item);
    bool Put(const Sigblock& item);
};

struct SigblockTripleBuffer_v1 {	//Ring buffer without FIFO semantics (Get operation reads any of last writes)
	static const unsigned int THREE = 3;
	enum BufferStatus {FREE, FULL, WRITING, READING};
    std::size_t sizeOfSigblock;
    unsigned int status[3]; //The status could be placed at the beginning of buffers, too.
    char buffer[]; //Array of 3 Sigblocks
    void Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock);
    bool Get(Sigblock& item);
    bool Put(const Sigblock& item);
};

struct SigblockTripleBuffer_v2 {	//Ring buffer without FIFO semantics (Get operation reads any of last writes)
	static const unsigned int THREE = 3;
	enum BufferStatus {FREE, FULL, WRITING, READING};
    std::size_t sizeOfSigblock;
    unsigned int lastWrittenBuffer;
    unsigned int status[3]; //the status could be placed at the beginning of buffers
    char buffer[]; //Array of 3 Sigblocks
    void Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock);
    bool Get(Sigblock& item);
    bool Put(const Sigblock& item);
};

struct SigblockRingBuffer {	//Ring buffer with FIFO semantics
    unsigned int readCount;
    unsigned int writeCount;
    unsigned int bufferSize;
    std::size_t sizeOfSigblock;
    char buffer[]; //Array of N Sigblock
    void Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock);
    bool Get(Sigblock& item);
    bool Put(const Sigblock& item);
};

#endif /* SOURCE_COMPONENTS_DATASOURCES_EPICSDATASOURCE_SIGBLOCKDOUBLEBUFFER_SNIPPET_H_ */
