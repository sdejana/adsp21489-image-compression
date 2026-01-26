# Image Compression using Discrete Cosine Transform on ADSP-21489

---

## Project Overview

The goal of this project is to implement a **grayscale image compression system** on the ADSP-21489 development platform using the **Discrete Cosine Transform (DCT)**. The compression is based on the **JPEG standard**, but only certain steps of the method will be implemented.

### General JPEG Compression Steps
1. Color space conversion from RGB to YCbCr.
2. Chrominance subsampling.
3. Image segmentation into 8×8 blocks.
4. Centering pixel values around zero.
5. Applying 2D DCT to each block.
6. Quantization of DCT coefficients using a quantization matrix.
7. Zig-zag permutation to form a 1D array.
8. Predictive coding of DC coefficients.
9. Run-length encoding of AC coefficients.
10. Entropy coding (Huffman algorithm).
11. Forming the final `.jpg` file.

### Project-Specific Implementation
In this project, the following simplified approach is used:
- Convert RGB images to **grayscale** and transfer them to the development platform.
- Center pixel values around zero, segment the image, and apply **DCT and quantization**.  
  - Quantization matrix can be chosen arbitrarily, but it will affect compression rate.  
  - The same matrix is needed for decompression.
- Apply **zig-zag transformation** and encode DC and AC coefficients.  
- **Entropy coding is not required.**  
- Serialize the results in any format.  
- Analyze results in **Python or MATLAB**, calculating metrics like **MSE** compared to the original image.

---
## Project Workflow

1. Load RGB image onto ADSP-21489 and convert to grayscale.
2. Center pixel values, segment image into blocks.
3. Apply DCT and quantization.
4. Perform zig-zag transformation.
5. Encode DC and AC coefficients (run-length for AC, differential for DC).
6. Serialize results in chosen format.
7. Analyze compression results in Python or MATLAB:
   - Calculate **MSE** compared to original image.
   - Test different optimization approaches.
8. Identify **bottle-necks** and measure number of cycles for major functions.
9. Optimize memory usage and execution speed.

---

## Additional Features

- Display processing progress using LEDs.
---

## Hardware and Software Requirements
- **Development Platform:** ADSP-21489
- **Software:** Python for analysis, CrossCore Embedded Studio for writing C program and uploading dxe file to board.
- **Version Control:** Git repository for all project code and documentation.

---

## References
- JPEG Compression Standard
- Discrete Cosine Transform (DCT) theory
- DSP development on ADSP-21489

--- 
---
## How to Use

### 1. Prerequisites
Before running the project, ensure you have the following:
- **ADSP-21489 development board** with necessary cables and power supply.
- **CrossCore Embedded Studio 20.12.1** installed for writing main program.
- **Python** installed for analyzing results.
- **Git** installed to clone the repository.

---

### 2. Preparing the Image
1. Choose the image(s) you want to compress. Supported formats: `.bmp`.
2. Transfer the image(s) to `images/` folder.
3. Compile provided bmp_loader project
   ```
   gcc main_menu.c bmp_loader.c -o main_menu
   ```
4. Make sure to set correct path to \
   ```const char* file_name = "./images/img_name.bmp";``` and \
   ```char* path = "path/to/your/crosscore/project/src/image_rgb";```
4. Run ```main_menu``` and wait for _RGB header generated successfully_ message or check errors if any show
---

### 3. Running the CrossCore project
1. Make sure to copy provided ```C``` files in ```project``` folder to ```src``` folder of your created ```CrossCore``` project
2. Make sure to place ```app.ldf``` file in correct folder (```project_name/system/startup_ldf/``)
3. Compile program and upload ```dxe``` file to ADSP21489 
4. The program will:
   - Center pixel values around zero.
   - Segment the image into 8×8 blocks.
   - Apply DCT and quantization using the predefined quantization matrix.
   - Perform zig-zag (Cik-Cak) ordering.
   - Encode DC and AC coefficients (DC: predictive, AC: run-length coding).
   - Serialize the compressed data to the output file in ```Debug``` folder of your project.
5. Wait until program finishes
---

### 4. Analyzing the Results
1. Make sure to set ```img_path = './images/img_name.bmp'``` correct
2. Make sure to provide correct path to function argument ```blocks = read_rle_file('/path/to/your/project/Debug/rle_dump.txt')```
3. Open ```CMD``` or preffered environment and run the provided ```dekoder.py``` script after main program execution finishes:
   ```bash
   python dekoder.py

