#include "async.h"

Async::Async(unsigned int sizePool) {
	nodePool = (ScheduleNode*) malloc(sizeof(ScheduleNode)*sizePool);
	for(unsigned int i = 0; i < sizePool; i++) {
        *(nodePool + i) = {
            .lastExecution = 0,
            .interval = 0,
            .isLoop = false,
            .finished = true,
            .function = nullptr
		};
	}
	nNodes = 0;
	this->sizePool = sizePool;
}

int Async::setInterval(void (*fun)(void), unsigned long time) {
	if(fun == nullptr || nNodes >= sizePool)
		return -1;

	unsigned int i = 0;
	for(; i < sizePool; i++) {
        if((nodePool + i)->finished) {
            *(nodePool + i) = {
                .lastExecution = millis(),
                .interval = time,
                .isLoop = true,
                .finished = false,
                .function = fun
            };
            break;
        }
	}

	nNodes++;
	return i;
}

int Async::setTimeout(void (*fun)(void), unsigned long time) {
	if(fun == nullptr || nNodes >= sizePool)
		return -1;

    unsigned int i = 0;
	for(; i < sizePool; i++) {
        if((nodePool + i)->finished) {
            *(nodePool + i) = {
                .lastExecution = millis(),
                .interval = time,
                .isLoop = false,
                .finished = false,
                .function = fun
            };
            break;
        }
	}

	nNodes++;
	return i;
}

bool Async::clearInterval(int id) {
    if(id < 0 || (unsigned int) id >= sizePool)
        return false;

    (nodePool + id)->finished = true;
    return true;
}

void Async::run() {
	for(unsigned int i = 0; i < sizePool; i++) {

        if(!(nodePool + i)->finished) {
           	unsigned long elapsedTime = (millis() - (nodePool + i)->lastExecution);
        	if(millis() < (nodePool + i)->lastExecution) // Overflow detected
        		elapsedTime = (0xFFFFFFFFL-(nodePool + i)->lastExecution) + millis();

        	if(elapsedTime >= (nodePool + i)->interval) {
	            (nodePool + i)->function();
	            if(!(nodePool + i)->isLoop) {
	                (nodePool + i)->finished = true;
	                nNodes--;
	            } else {
	                (nodePool + i)->lastExecution = millis();
	            }
	        } // End elapsed time greater than interval of node
        } // End if node not finished

	} // End for
}

Async::~Async() {
	free(nodePool);
}
