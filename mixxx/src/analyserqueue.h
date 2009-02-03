#ifndef ANALYSERQUEUE_H
#define ANALYSERQUEUE_H

#include <QList>
#include <QThread>
#include <QQueue>
#include <QWaitCondition>

#include "configobject.h"
#include "analyser.h"
#include "trackinfoobject.h"

class SoundSourceProxy;

class AnalyserQueue : public QThread {
    Q_OBJECT

public:
	AnalyserQueue();
	virtual ~AnalyserQueue();
    
	void queueAnalyseTrack(TrackInfoObject* tio);
	void stop();
    
	static AnalyserQueue* createDefaultAnalyserQueue(ConfigObject<ConfigValue> *_config);
	static AnalyserQueue* createBPMAnalyserQueue(ConfigObject<ConfigValue> *_config);
    static AnalyserQueue* createAnalyserQueue(QList<Analyser*> analysers);
    
signals:
    void trackProgress(TrackInfoObject*,int);
    void trackFinished(TrackInfoObject*);
    
protected:
	void run();
    
private:
	void addAnalyser(Analyser* an);
    
	QList<Analyser*> m_aq;
    
	TrackInfoObject* dequeueNextBlocking();
	void doAnalysis(TrackInfoObject* tio, SoundSourceProxy *pSoundSource);
    
	bool m_exit;
    
	// The processing queue and associated mutex
	QQueue<TrackInfoObject*> m_tioq;
	QMutex m_qm;
	QWaitCondition m_qwait;
};

#endif
