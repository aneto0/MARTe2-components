#include "SigblockDoubleBuffer.snippet.h"

void SigblockDoubleBuffer_v1::Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock) {
	//this->bufferSize = bufferSize;
	this->sizeOfSigblock = sizeOfSigblock;
	status[0] = FREE;
	status[1] = FREE;
}

bool SigblockDoubleBuffer_v1::Get(Sigblock& sb) {
	bool fret = true;
	unsigned int i = 0;
	bool found = false;
	while (!found && (i<TWO)) {
		found = CAS(&(status[i]), FULL, READING);
		i++;
	}
	if (found) {
		//ASSERT: status[i-1] == READING;
		std::memcpy(&sb, &(buffer[sizeOfSigblock * (i-1)]), sizeOfSigblock);
		/*ATOMIC*/status[i-1] = FREE;
	}
	else {
		fret = false;
	}
	return fret;
}

bool SigblockDoubleBuffer_v1::Put(const Sigblock& sb) {
	bool fret = true;
	unsigned int i = 0;
	bool found = false;
	while (!found && (i<TWO)) {
		found = (CAS(&(status[i]), FREE, WRITING) || CAS(&(status[i]), FULL, WRITING));
		i++;
	}
	//ASSERT: found must be true, otherwise there is an flaw in the algorithm or data structure.
	if (found) {
		//ASSERT: status[i-1] == WRITING;
		std::memcpy(&(buffer[sizeOfSigblock * (i-1)]), &sb, sizeOfSigblock);
		/*ATOMIC*/status[i-1] = FULL;
	}
	else {
		fret = false;
	}
	return fret;
}

void SigblockTripleBuffer_v1::Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock) {
	//this->bufferSize = bufferSize;
	this->sizeOfSigblock = sizeOfSigblock;
	status[0] = FREE;
	status[1] = FREE;
	status[2] = FREE;
}

bool SigblockTripleBuffer_v1::Get(Sigblock& sb) {
	bool fret = true;
	unsigned int i = 0;
	bool found = false;
	while (!found && (i<THREE)) {
		found = CAS(&(status[i]), FULL, READING);
		i++;
	}
	if (found) {
		//ASSERT: status[i-1] == READING;
		std::memcpy(&sb, &(buffer[sizeOfSigblock * (i-1)]), sizeOfSigblock);
		/*ATOMIC*/status[i-1] = FREE;
	}
	else {
		fret = false;
	}
	return fret;
}

bool SigblockTripleBuffer_v1::Put(const Sigblock& sb) {
	bool fret = true;
	unsigned int i = 0;
	bool found = false;
	while (!found && (i<THREE)) {
		found = (CAS(&(status[i]), FREE, WRITING) || CAS(&(status[i]), FULL, WRITING));
		i++;
	}
	//ASSERT: found must be true, otherwise there is an flaw in the algorithm or data structure.
	if (found) {
		//ASSERT: status[i-1] == WRITING;
		std::memcpy(&(buffer[sizeOfSigblock * (i-1)]), &sb, sizeOfSigblock);
		/*ATOMIC*/status[i-1] = FULL;
	}
	else {
		fret = false;
	}
	return fret;
}

void SigblockTripleBuffer_v2::Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock) {
	//this->bufferSize = bufferSize;
	this->sizeOfSigblock = sizeOfSigblock;
	lastWrittenBuffer = THREE;
	status[0] = FREE;
	status[1] = FREE;
	status[2] = FREE;
}

bool SigblockTripleBuffer_v2::Get(Sigblock& sb) {
	bool fret = true;
	unsigned int count = 0;
	unsigned int i = lastWrittenBuffer;
	bool found = false;
	while (!found && (count<THREE)) {
		found = CAS(&(status[i]), FULL, READING);
		count++;
		i = ((i + 1) % THREE);
	}
	if (found) {
		//ASSERT: status[i-1] == READING;
		unsigned int target = ((i - 1) % THREE);
		std::memcpy(&sb, &(buffer[sizeOfSigblock * target]), sizeOfSigblock);
		/*ATOMIC*/status[target] = FREE;
	}
	else {
		fret = false;
	}
	return fret;
}

bool SigblockTripleBuffer_v2::Put(const Sigblock& sb) {
	bool fret = true;
	unsigned int count = 0;
	unsigned int i = ((lastWrittenBuffer + 1) % THREE);
	bool found = false;
	while (!found && (count<THREE)) {
		found = CAS(&(status[i]), FREE, WRITING) || CAS(&(status[i]), FULL, WRITING);
		count++;
		i = ((i + 1) % THREE);
	}
	//ASSERT: found must be true, otherwise there is an flaw in the algorithm or data structure.
	if (found) {
		//ASSERT: status[i-1] == WRITING;
		unsigned int target = ((i - 1) % THREE);
		std::memcpy(&(buffer[sizeOfSigblock * target]), &sb, sizeOfSigblock);
		/*ATOMIC*/status[target] = FULL;
		lastWrittenBuffer = target;
	}
	else {
		fret = false;
	}
	return fret;
}

void SigblockRingBuffer::Reset(const unsigned int bufferSize, std::size_t sizeOfSigblock) {
	this->readCount = 0;
	this->writeCount = 0;
	this->bufferSize = bufferSize;
	this->sizeOfSigblock = sizeOfSigblock;
}

bool SigblockRingBuffer::Get(Sigblock& sb) {
    bool isEmpty = ((writeCount - readCount) == 0);
    if (!isEmpty) {
        std::memcpy(&sb, &(buffer[sizeOfSigblock * (readCount % bufferSize)]), sizeOfSigblock);
        asm volatile("" ::: "memory");	//memory barrier	__sync_synchronize() ??
        ++readCount; /*__sync_fetch_and_add ??*/
    }
    return !isEmpty;
}

bool SigblockRingBuffer::Put(const Sigblock& sb) {
    bool isFull = ((writeCount - readCount) == bufferSize);
    if (!isFull) {
        std::memcpy(&(buffer[sizeOfSigblock * (writeCount % bufferSize)]), &sb, sizeOfSigblock);
        asm volatile("" ::: "memory");	//memory barrier	__sync_synchronize() ??
        ++writeCount; /*__sync_fetch_and_add ??*/
    }
    return !isFull;
}
