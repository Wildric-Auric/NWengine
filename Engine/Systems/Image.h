#pragma once

#include <string>
#include <Globals.h>

#include "Asset.h"


/**
 * @brief Represents an image asset.
 */
class Image : public Asset {
public:
    std::string path = ""; /**< The file path of the image. */
    uint8* pixelBuffer = nullptr; /**< The pixel buffer of the image. */
    int channels = 0; /**< The number of color channels in the image. */
    int width = 0; /**< The width of the image. */
    int height = 0; /**< The height of the image. */
    bool alpha = 0; /**< Indicates whether the image has an alpha channel. */
    
    /**
     * @brief Default constructor for Image.
     */
    Image() = default;
    
    /**
     * @brief Constructor for Image.
     * @param x The file path of the image.
     * @param buf The pixel buffer of the image.
     * @param ch The number of color channels in the image.
     * @param w The width of the image.
     * @param h The height of the image.
     * @param a Indicates whether the image has an alpha channel.
     */
    Image(const std::string& x, uint8* buf, int ch, int w, int h, bool a) : path{x}, pixelBuffer{buf}, channels{ch},
    width{w}, height{h}, alpha{a} {};
    
    /**
     * @brief Loads an image from a file.
     * @param path The file path of the image.
     * @param output The output buffer.
     * @return A pointer to the loaded image asset.
     */
    Asset* LoadFromFile(const char* path, void* output) override;
    
    /**
     * @brief Cleans up the image asset.
     */
    void Clean() override;
};
