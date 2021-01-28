__kernel void eigenFaceSearch(
    __read_only image2d_t testImage,        // Input test image
    __write_only image2d_t projectionImage, // Output projection image
    int rows,                               // Number of rows in the test image
    int cols,                               // Number of columns in the test image
    __constant float* eigenFace,            // The first eigenface buffer
    sampler_t sampler                       // Sampler for reading the image data
    )
  {
    int myCol = get_global_id(0);           // Column index of test image pixel
    int myRow = get_global_id(1);           // Row index of test image pixel
    float4 dotp = {0.0f, 0.0f, 0.0f, 0.0f}; // Resulting dot project variable
    float4 pixel = {0.0f, 0.0f, 0.0f, 0.0f};// Variable for reading the pixel
    float sum = 0.f;                        // Sum of the pixels in the test region
    float meanVal=0.f;
    float patch[4096];                      // Local buffer to hold the mean
                                            // subtracted region
    int2 myCoords;                          // Local variable for holding current
                                            // pixels coords
    myCoords.x = myCol;                    
    myCoords.y = myRow;
    if (!(( myCol > cols-64) || (myRow > rows - 64))) { // Bounds check
      for (int i = 0; i < 64; i++) { // Loop over every pixel in test region
        int2 coords;
        coords.y = myRow + i;
        for (int j = 0; j < 64; j++) {
          coords.x = myCol + j;
          // Read the pixel at coords, from the test image using the sampler
          pixel = read_imagef(testImage, sampler, coords); 
          patch[i*64+j] = pixel.x;  // Take the first component as it is a single
                                    // float buffer
          sum += patch[i*64+j];     // Accumulate the sum
        } 
  }  
    meanVal = sum / 256.;           // An intensity factor proportional to sum.
    for (int i= 0; i < 4096; i++){  // Remove from each pixel
      patch[i] -= meanVal;          // Per component subtract of intensity factor
    }
    // Perform the dot product
    for (int i= 0; i < 4096; i++){
      dotp.x += patch[i] * eigenFace[i]; // Do the component-wise dot product
    }
    // Write back the dot product into the projection Image
    write_imagef(projectionImage, myCoords, dotp);
  }
}