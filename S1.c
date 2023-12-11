#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

struct PaperNode {
    int rollNumber;
    float thickness;
    float weight;
    struct PaperNode* left;
    struct PaperNode* right;
};

float getValueThickness(struct PaperNode* node);
float getValueWeight(struct PaperNode* node);

void calculate(struct PaperNode* node, float (*getValue)(struct PaperNode* node), int* count, float* sum) {
    if (node != NULL) {
        calculate(node->left, getValue, count, sum);

        *sum += getValue(node);
        (*count)++;

        calculate(node->right, getValue, count, sum);
    }
}

void storeValues(struct PaperNode* node, float (*getValue)(struct PaperNode* node), int* count, float* values) {
    if (node != NULL) {
        storeValues(node->left, getValue, count, values);

        values[*count] = getValue(node);
        (*count)++;

        storeValues(node->right, getValue, count, values);
    }
}

void findMinMax(struct PaperNode* node, float (*getValue)(struct PaperNode* node), float* minVal, float* maxVal) {
    if (node != NULL) {
        findMinMax(node->left, getValue, minVal, maxVal);

        float value = getValue(node);
        *minVal = fmin(*minVal, value);
        *maxVal = fmax(*maxVal, value);

        findMinMax(node->right, getValue, minVal, maxVal);
    }
}


float calculateAverage(struct PaperNode* root, float (*getValue)(struct PaperNode* node)) {
    int count = 0;
    float sum = 0.0;

    calculate(root, getValue, &count, &sum);

    if (count > 0) {
        return sum / count;
    } else {
        return 0.0;
    }
}

float calculateVariance(struct PaperNode* root, float mean, float (*getValue)(struct PaperNode* node)) {
    int count = 0;
    float sumSquaredDiff = 0.0;

    calculate(root, getValue, &count, &sumSquaredDiff);

    if (count > 0) {
        return sumSquaredDiff / count;
    } else {
        return 0.0;
    }
}

float calculateStandardDeviation(float variance) {
    return sqrt(variance);
}

float calculateMedian(struct PaperNode* root, float (*getValue)(struct PaperNode* node)) {
    int count = 0;
    int i = 0;
    float values[100];

    storeValues(root, getValue, &count, values);

    if (count == 0) {
        return 0.0;
    }

  
    for (int j = 0; j < count - 1; j++) {
        for (int k = 0; k < count - j - 1; k++) {
            if (values[k] > values[k + 1]) {
                float temp = values[k];
                values[k] = values[k + 1];
                values[k + 1] = temp;
            }
        }
    }


    if (count % 2 == 0) {
        return (values[count / 2 - 1] + values[count / 2]) / 2;
    } else {
        return values[count / 2];
    }
}

float calculateRange(struct PaperNode* root, float (*getValue)(struct PaperNode* node)) {
   
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;

    findMinMax(root, getValue, &minVal, &maxVal);

   
}

float calculateCoefficientOfVariation(float mean, float stdDeviation) {
    if (mean > 0.0) {
        return (stdDeviation / mean) * 100;
    } else {
        return 0.0;
    }
}


float getValueThickness(struct PaperNode* node) {
    return node->thickness;
}


float getValueWeight(struct PaperNode* node) {
    return node->weight;
}

struct PaperNode* createPaperNode(int rollNumber, float thickness, float weight) {
    struct PaperNode* newNode = (struct PaperNode*)malloc(sizeof(struct PaperNode));
    newNode->rollNumber = rollNumber;
    newNode->thickness = thickness;
    newNode->weight = weight;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct PaperNode* insertPaperRoll(struct PaperNode* root, int rollNumber, float thickness, float weight) {
    if (root == NULL) {
        return createPaperNode(rollNumber, thickness, weight);
    }

    if (rollNumber < root->rollNumber) {
        root->left = insertPaperRoll(root->left, rollNumber, thickness, weight);
    } else if (rollNumber > root->rollNumber) {
        root->right = insertPaperRoll(root->right, rollNumber, thickness, weight);
    }

    return root;
}

void printRollInformation(struct PaperNode* node) {
    printf("Roll Number: %d, Thickness: %.2f, Weight: %.2f\n", node->rollNumber, node->thickness, node->weight);
}

int main() {
    struct PaperNode* root = NULL;

    int numRolls;
    printf("Enter the number of paper rolls: ");
    scanf("%d", &numRolls);

    for (int i = 0; i < numRolls; i++) {
        int rollNumber;
        float thickness, weight;

        printf("Enter Roll Number for Roll %d: ", i + 1);
        scanf("%d", &rollNumber);

        printf("Enter Thickness for Roll %d: ", i + 1);
        scanf("%f", &thickness);

        printf("Enter Weight for Roll %d: ", i + 1);
        scanf("%f", &weight);

        root = insertPaperRoll(root, rollNumber, thickness, weight);
    }

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Average Thickness\n");
        printf("2. Average Weight\n");
        printf("3. Variance Thickness\n");
        printf("4. Variance Weight\n");
        printf("5. Standard Deviation Thickness\n");
        printf("6. Standard Deviation Weight\n");
        printf("7. Median Thickness\n");
        printf("8. Median Weight\n");
        printf("9. Range Thickness\n");
        printf("10. Range Weight\n");
        printf("11. Coefficient of Variation Thickness\n");
        printf("12. Coefficient of Variation Weight\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Average Thickness: %.2f\n", calculateAverage(root, &getValueThickness));
                break;
            case 2:
                printf("Average Weight: %.2f\n", calculateAverage(root, &getValueWeight));
                break;
            case 3:
                printf("Variance Thickness: %.2f\n", calculateVariance(root, calculateAverage(root, &getValueThickness), &getValueThickness));
                break;
            case 4:
                printf("Variance Weight: %.2f\n", calculateVariance(root, calculateAverage(root, &getValueWeight), &getValueWeight));
                break;
            case 5:
                printf("Standard Deviation Thickness: %.2f\n", calculateStandardDeviation(calculateVariance(root, calculateAverage(root, &getValueThickness), &getValueThickness)));
                break;
            case 6:
                printf("Standard Deviation Weight: %.2f\n", calculateStandardDeviation(calculateVariance(root, calculateAverage(root, &getValueWeight), &getValueWeight)));
                break;
            case 7:
                printf("Median Thickness: %.2f\n", calculateMedian(root, &getValueThickness));
                break;
            case 8:
                printf("Median Weight: %.2f\n", calculateMedian(root, &getValueWeight));
                break;
            case 9:
                printf("Range Thickness: %.2f\n", calculateRange(root, &getValueThickness));
                break;
            case 10:
                printf("Range Weight: %.2f\n", calculateRange(root, &getValueWeight));
                break;
            case 11:
                printf("Coefficient of Variation Thickness: %.2f%%\n", calculateCoefficientOfVariation(calculateAverage(root, &getValueThickness), calculateStandardDeviation(calculateVariance(root, calculateAverage(root, &getValueThickness), &getValueThickness))));
                break;
            case 12:
                printf("Coefficient of Variation Weight: %.2f%%\n", calculateCoefficientOfVariation(calculateAverage(root, &getValueWeight), calculateStandardDeviation(calculateVariance(root, calculateAverage(root, &getValueWeight), &getValueWeight))));
                break;
            case 0:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 0);

    return 0;
}