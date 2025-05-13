#include <stdio.h>
#include <string.h>
#include "feedback.h"
#include "utils.h"

Feedback feedbacks[100];
int feedbackCount = 0;

void loadFeedbacks() {
    FILE *fp = fopen("feedback.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%d %s %[^\n]", &feedbacks[feedbackCount].feedbackId, feedbacks[feedbackCount].customerName,
                  feedbacks[feedbackCount].message) != EOF) {
        feedbackCount++;
    }
    fclose(fp);
}

void saveFeedback(Feedback f) {
    FILE *fp = fopen("feedback.txt", "a");
    fprintf(fp, "%d %s %s\n", f.feedbackId, f.customerName, f.message);
    fclose(fp);
}

void giveFeedback(char customerName[]) {
    Feedback f;
    f.feedbackId = feedbackCount + 1;
    strcpy(f.customerName, customerName);
    getchar(); // Flush newline
    printf("Enter feedback: ");
    fgets(f.message, 200, stdin);
    f.message[strcspn(f.message, "\n")] = 0;

    feedbacks[feedbackCount++] = f;
    saveFeedback(f);
    printf(GREEN "Feedback recorded. Thank you!\n" RESET);
}

void viewFeedback() {
    printf(YELLOW "\n--- Feedbacks ---\n" RESET);
    for (int i = 0; i < feedbackCount; i++) {
        printf("%d. %s - %s\n", feedbacks[i].feedbackId, feedbacks[i].customerName, feedbacks[i].message);
    }
}
