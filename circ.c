#include <stdio.h>
#include <math.h>

void drawCircle(int radius) {
    // Dimensions of the output area
    int diameter = 2 * radius + 1;

    for (int y = 0; y < diameter; y++) {
        for (int x = 0; x < diameter; x++) {
            // Calculate distance from the center
            float dx = x - radius;
            float dy = y - radius;
            float distance = sqrt(dx * dx + dy * dy);

            // Print a character if the point is on the circle's boundary
            if (distance > radius - 0.5 && distance < radius + 0.5)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

int main() {
    int radius;
    printf("Enter the radius of the circle: ");
    scanf("%d", &radius);

    drawCircle(radius);
    return 0;
}
