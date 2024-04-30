#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void resizeImage(const char* inputFileName, const char* outputFileName, int originalWidth, int originalHeight, int newWidth, int newHeight);

int main()
{
    // Input FileName name hard coded because I could not get it to cin the file name.
    // Input FileName must be changed in the code to the name of the input file to get the desired result
    const char* inputFileName = "museum.raw";
    const char* outputFileName = "resized_picture_question2e.raw";
    int originalWidth, originalHeight, newWidth, newHeight;

    // Enter the size of the picture that is being used
    cout << "\nEnter the Original Width and Height of the picture respectively" << endl;
    cin >> originalWidth;
    cin >> originalHeight;

    // Enter the new size of the picture that is to be achieved after resizing 
    cout << "\nEnter the New Width and Height of the picture respectively" << endl;
    cin >> newWidth;
    cin >> newHeight;

    // Resize the image and save it to the output file
    resizeImage(inputFileName, outputFileName, originalWidth, originalHeight, newWidth, newHeight);

    cout << "Image resized and saved to " << outputFileName << endl;

    return 0;
}

// Function to resize an image using weighted average resampling
void resizeImage(const char* inputFileName, const char* outputFileName, int originalWidth, int originalHeight, int newWidth, int newHeight) 
{
    // Calculate the scaling factors for width and height
    double scaleX = (double)newWidth / originalWidth;
    double scaleY = (double)newHeight / originalHeight;

    // Open the input file for reading
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Error: Cannot open input file." << endl;
        return;
    }

    // Open the output file for writing
    ofstream outputFile(outputFileName, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Error: Cannot create output file." << endl;
        return;
    }

    // Calculate the new image data size
    int newSize = newWidth * newHeight;

    // Read the input image data into a vector
    vector<unsigned char> imageData(originalWidth * originalHeight);
    inputFile.read(reinterpret_cast<char*>(&imageData[0]), imageData.size());

    // Create a vector for the resized image data
    vector<unsigned char> resizedImageData(newSize);

    // Perform weighted average resampling
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = x / scaleX;
            double srcY = y / scaleY;

            int x1 = static_cast<int>(srcX);
            int x2 = min(x1 + 1, originalWidth - 1);
            int y1 = static_cast<int>(srcY);
            int y2 = min(y1 + 1, originalHeight - 1);

            double weightX1 = x2 - srcX;
            double weightX2 = srcX - x1;
            double weightY1 = y2 - srcY;
            double weightY2 = srcY - y1;

            double interpolatedValue = 0.0;

            interpolatedValue += imageData[y1 * originalWidth + x1] * (weightX1 * weightY1);
            interpolatedValue += imageData[y1 * originalWidth + x2] * (weightX2 * weightY1);
            interpolatedValue += imageData[y2 * originalWidth + x1] * (weightX1 * weightY2);
            interpolatedValue += imageData[y2 * originalWidth + x2] * (weightX2 * weightY2);

            resizedImageData[y * newWidth + x] = static_cast<unsigned char>(interpolatedValue);
        }
    }

    // Write the resized image data to the output file
    outputFile.write(reinterpret_cast<char*>(&resizedImageData[0]), newSize);
}