#ifndef _PROGRESSLISTENER_H
#define _PROGRESSLISTENER_H

class ProgressListener {

public:
	ProgressListener() { }

	virtual void updateProgress(double percentage) = 0;

	virtual ~ProgressListener() { }
};

#endif
