#ifndef FEEDBACK_H
#define FEEDBACK_H

typedef struct {
    int feedbackId;
    char customerName[50];
    char message[200];
} Feedback;

extern Feedback feedbacks[];
extern int feedbackCount;

void loadFeedbacks();
void saveFeedback(Feedback f);
void giveFeedback(char customerName[]);
void viewFeedback();

#endif
