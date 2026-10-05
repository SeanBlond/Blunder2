#include "GraphicsRenderer.h"
#include "GraphicsDevice.h"

// Data Management Function definitions
bool GraphicsRenderer::init(
    std::string uiTextureAtlasCoords, std::string uiTextureAtlasImage,
    std::string textTextureAtlasCoords, std::string textTextureAtlasImage,
    float uiScale)
{
    // Creating the PosColor mesh buffer
    mpPosColorBuffer = new PosColorMeshBuffer();
    if (!mpPosColorBuffer->init())
    {
        std::cout << "Failed to initialize PosColor mesh buffer" << std::endl;
        return false;
    }

    // Creating the UI Texture mesh buffer
    mpUITextureBuffer = new UITextureMeshBuffer();
    if (!mpUITextureBuffer->init(uiTextureAtlasCoords, uiTextureAtlasImage))
    {
        std::cout << "Failed to initialize UI Texture mesh buffer" << std::endl;
        return false;
    }

    // Creating the Text mesh buffer
    mpTextBuffer = new TextMeshBuffer();
    if (!mpTextBuffer->init(textTextureAtlasCoords, textTextureAtlasImage))
    {
        std::cout << "Failed to initialize Text mesh buffer" << std::endl;
        return false;
    }
    
    // Creating the GameObject buffer
    mpGameObjectBuffer = new GameObjectBuffer();
    if (!mpGameObjectBuffer->init())
    {
        std::cout << "Failed to initialize GameObject buffer" << std::endl;
        return false;
    }

    // Creating the depth texture
    updateDepthTexture((smath::ivec2)GraphicsDevice::getInstance()->getDisplayDimensions());

    // Setting the UI Scale
    mUIScale = uiScale;

    // Returning true for a succesful initialization
    return true;
}
void GraphicsRenderer::cleanup()
{
    // Deallocating the Buffers
    delete mpPosColorBuffer;
    mpPosColorBuffer = nullptr;
    delete mpUITextureBuffer;
    mpUITextureBuffer = nullptr;
    delete mpTextBuffer;
    mpTextBuffer = nullptr;
    delete mpGameObjectBuffer;
    mpGameObjectBuffer = nullptr;

    // Deallocating the depth texture
    SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpDepthTexture);
}


// Getter definitions
float GraphicsRenderer::getTextSize(const TextSize& size) const
{
    if (size == TEXT_SMALL)
        return mUIScale * 15.0f;
    if (size == TEXT_NORMAL)
        return mUIScale * 20.0f;
    if (size == TEXT_LARGE)
        return mUIScale * 25.0f;
    else
        return 0.0f;
}

// Function definitions for adding meshes to the PosColor buffer
void GraphicsRenderer::addMesh(const PosColorMesh& mesh)
{
    mpPosColorBuffer->addMesh(mesh);
}
void GraphicsRenderer::addLine(const smath::vec2& startPos, const smath::vec2& endPos, const float& layerOffset, const Color& color, const float& thickness)
{
    // Generating the necessary vectors for the line
    smath::vec2 lineRelative = smath::normalize(endPos - startPos);
    smath::vec2 lineOffset = smath::vec2(-lineRelative.y, lineRelative.x) * thickness * 0.5f;

    // Creating the mesh
    PosColorMesh tempMesh =
    {
        // Creating the vertices
        .vertices =
        {
            { smath::vec3(startPos + lineOffset, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(startPos - lineOffset, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(endPos - lineOffset,   mCurrentLayerValue + layerOffset), color },
            { smath::vec3(endPos + lineOffset,   mCurrentLayerValue + layerOffset), color },
        },

        // Creating the indices
        .indices =
        {
            0, 1, 2,
            0, 2, 3
        }
    };

    // Adding the mesh to the buffer
    addMesh(tempMesh);
}
void GraphicsRenderer::addRectangle(const SDL_FRect& rect, const float& layerOffset, const Color& color)
{
    // Creating the mesh
    PosColorMesh tempMesh =
    {
        // Creating the vertices
        .vertices =
        {
            { smath::vec3(rect.x,          rect.y,          mCurrentLayerValue + layerOffset), color },
            { smath::vec3(rect.x,          rect.y + rect.h, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(rect.x + rect.w, rect.y + rect.h, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(rect.x + rect.w, rect.y,          mCurrentLayerValue + layerOffset), color },
        },

        // Creating the indices
        .indices =
        {
            0, 1, 2,
            0, 2, 3
        }
    };

    // Adding the mesh to the buffer
    addMesh(tempMesh);
}
void GraphicsRenderer::addRectangle(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const Color& color)
{
    // Creating the mesh
    PosColorMesh tempMesh =
    {
        // Creating the vertices
        .vertices =
        {
            { smath::vec3(position.x,          position.y,          mCurrentLayerValue + layerOffset), color },
            { smath::vec3(position.x,          position.y + size.y, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(position.x + size.x, position.y + size.y, mCurrentLayerValue + layerOffset), color },
            { smath::vec3(position.x + size.x, position.y,          mCurrentLayerValue + layerOffset), color },
        },

        // Creating the indices
        .indices =
        {
            0, 1, 2,
            0, 2, 3
        }
    };

    // Adding the mesh to the buffer
    addMesh(tempMesh);
}
void GraphicsRenderer::addRectangleOutline(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const Color& color, const float& thickness)
{
    // Creating the corner positions
    smath::vec2 corners[] =
    {
        smath::vec2(position.x - (0.5f * size.x), position.y - (0.5f * size.y)),
        smath::vec2(position.x - (0.5f * size.x), position.y + (0.5f * size.y)),
        smath::vec2(position.x + (0.5f * size.x), position.y + (0.5f * size.y)),
        smath::vec2(position.x + (0.5f * size.x), position.y - (0.5f * size.y))
    };

    // Defining the thickness applied to the corners in each direction
    float t = 0.5f * thickness;

    // Creating the mesh w/ vertices
    PosColorMesh tempMesh =
    {
        // Creating the vertices
        .vertices =
        {
            { smath::vec3(corners[0] + smath::vec2(t),   mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[0] + smath::vec2(-t),  mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[1] + smath::vec2(t, -t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[1] + smath::vec2(-t, t),  mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[2] + smath::vec2(-t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[2] + smath::vec2(t),   mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[3] + smath::vec2(-t, t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[3] + smath::vec2(t, -t),  mCurrentLayerValue + layerOffset), color },
        }
    };

    // Defining the indices
    for (int i = 0; i < 8; i++)
    {
        tempMesh.indices.push_back(i);
        tempMesh.indices.push_back((i + 1) % 8);
        tempMesh.indices.push_back((i + 2) % 8);
    }

    // Adding the mesh
    addMesh(tempMesh);
}
void GraphicsRenderer::addRectangleOutline(const SDL_FRect& rect, const float& layerOffset, const Color& color, const float& thickness)
{
    // Creating the corner positions
    smath::vec2 corners[] =
    {
        smath::vec2(rect.x,          rect.y         ),
        smath::vec2(rect.x,          rect.y + rect.h),
        smath::vec2(rect.x + rect.w, rect.y + rect.h),
        smath::vec2(rect.x + rect.w, rect.y         )
    };

    // Defining the thickness applied to the corners in each direction
    float t = 0.5f * thickness;

    // Creating the mesh w/ vertices
    PosColorMesh tempMesh =
    {
        // Creating the vertices
        .vertices =
        {
            { smath::vec3(corners[0] + smath::vec2(t),   mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[0] + smath::vec2(-t),  mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[1] + smath::vec2(t, -t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[1] + smath::vec2(-t, t),  mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[2] + smath::vec2(-t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[2] + smath::vec2(t),   mCurrentLayerValue + layerOffset), color },

            { smath::vec3(corners[3] + smath::vec2(-t, t),  mCurrentLayerValue + layerOffset), color },
            { smath::vec3(corners[3] + smath::vec2(t, -t),  mCurrentLayerValue + layerOffset), color },
        }
    };

    // Defining the indices
    for (int i = 0; i < 8; i++)
    {
        tempMesh.indices.push_back(i);
        tempMesh.indices.push_back((i + 1) % 8);
        tempMesh.indices.push_back((i + 2) % 8);
    }

    // Adding the mesh
    addMesh(tempMesh);
}
void GraphicsRenderer::addCircle(const smath::vec2& center, const smath::vec2& radii, const float& layerOffset, const Color& color, const int& quality)
{
    // Clamping the quality between 3 and 128
    int actualQuality = smath::clamp(quality, 3, 128);

    // Creating the temp mesh
    PosColorMesh tempMesh;

    // Adding the origin vertice
    tempMesh.vertices.push_back(
        {
            smath::vec3(center, mCurrentLayerValue + layerOffset), color
        });

    // Generating the vertices for the circle
    for (int i = 0; i < actualQuality; i++)
    {
        // Calculating theta of the current iteratrion i
        float theta = (smath::TAU * (float)i) / (float)actualQuality;

        // Pushing back the vertice
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + radii.x * cos(theta), center.y + radii.y * sin(theta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });
    }

    // Creating the indices for each triangle of the circle
    for (int i = 1; i < actualQuality; i++)
    {
        tempMesh.indices.push_back(0);
        tempMesh.indices.push_back(i);
        tempMesh.indices.push_back(i + 1);
    }
    // Adding the final triangle indices
    tempMesh.indices.push_back(0);
    tempMesh.indices.push_back(actualQuality);
    tempMesh.indices.push_back(1);

    // Adding the temp mesh
    addMesh(tempMesh);
}
void GraphicsRenderer::addCircle(const smath::vec2& center, const float& radius, const float& layerOffset, const Color& color, const int& quality)
{
    // Calling the other addCircle function
    addCircle(center, smath::vec2(radius), layerOffset, color, quality);
}
void GraphicsRenderer::addCircleOutline(const smath::vec2& center, const smath::vec2& radii, const float& layerOffset, const Color& color, const float& thickness, const int& quality)
{
    // Clamping the quality between 3 and 128
    int actualQuality = smath::clamp(quality, 3, 128);

    // Creating the temp mesh
    PosColorMesh tempMesh;


    // Generating the vertices for the circle
    for (int i = 0; i < actualQuality; i++)
    {
        // Calculating theta of the current iteratrion i
        float theta = (smath::TAU * (float)i) / (float)actualQuality;
        float prevTheta = (smath::TAU * (float)(i - 1)) / (float)actualQuality;

        // Calculating the start and end pos of the line segment of the circle
        smath::vec2 startPos = smath::vec2(center.x + radii.x * cos(prevTheta), center.y + radii.y * sin(prevTheta));
        smath::vec2 endPos = smath::vec2(center.x + radii.x * cos(theta), center.y + radii.y * sin(theta));

        // Generating the necessary vectors for the line
        smath::vec2 lineRelative = smath::normalize(endPos - startPos);
        smath::vec2 lineOffset = smath::vec2(-lineRelative.y, lineRelative.x) * thickness * 0.5f;

        // Pushing back the vertice
        tempMesh.vertices.push_back({ smath::vec3(endPos + lineOffset, mCurrentLayerValue + layerOffset), color }); // Inner vertice
        tempMesh.vertices.push_back({ smath::vec3(endPos - lineOffset, mCurrentLayerValue + layerOffset), color }); // Outer vertice
    }

    // Creating the indices for each triangle of the circle
    for (int i = 0; i < actualQuality * 2; i++)
    {
        // Inner Triangle
        tempMesh.indices.push_back(i);
        tempMesh.indices.push_back((i + 1) % (actualQuality * 2));
        tempMesh.indices.push_back((i + 2) % (actualQuality * 2));
    }


    // Adding the temp mesh
    addMesh(tempMesh);
}
void GraphicsRenderer::addCircleOutline(const smath::vec2& center, const float& radius, const float& layerOffset, const Color& color, const float& thickness, const int& quality)
{
    addCircleOutline(center, smath::vec2(radius), layerOffset, color, thickness, quality);
}
void GraphicsRenderer::addStar(const smath::vec2& center, const float& outerRadius, const float& layerOffset, const Color& color)
{
    // Creating the temp mesh
    PosColorMesh tempMesh;

    // Generating the vertices for the circle
    for (int i = 0; i < 5; i++)
    {
        // Calculating theta of the current iteratrion i
        float outerTheta = smath::TAU * ((float)i / 5.0f) - (smath::PI / 10.0f);
        float innerTheta = smath::TAU * ((float)i / 5.0f) + (smath::PI / 10.0f);

        // Outer vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + outerRadius * cos(outerTheta), center.y + outerRadius * sin(outerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });

        // Inner vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + outerRadius * 0.38f * cos(innerTheta), center.y + outerRadius * 0.38f * sin(innerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });
    }

    // Adding the origin vertice
    tempMesh.vertices.push_back(
        {
            smath::vec3(center, mCurrentLayerValue + layerOffset), color
        });

    // Creating the indices for the triangles
    for (int i = 0; i < 10; i += 2)
    {
        int vertexA = (i + 9) % 10;
        int vertexB = i;
        int vertexC = (i + 1) % 10;

        // Outer triangle
        tempMesh.indices.push_back(vertexA);
        tempMesh.indices.push_back(vertexB);
        tempMesh.indices.push_back(vertexC);

        // Inner triangle
        tempMesh.indices.push_back(vertexA);
        tempMesh.indices.push_back(10);
        tempMesh.indices.push_back(vertexC);
    }

    // Adding the temp mesh
    addMesh(tempMesh);
}
void GraphicsRenderer::addStarOutline(const smath::vec2& center, const float& outerRadius, const float& layerOffset, const Color& color, const float& thickness)
{
    // Creating the temp mesh
    PosColorMesh tempMesh;

    // Defining how much the radii will be effected by the thickness
    float thicknessOffset = thickness * 0.5f;

    // Generating the vertices for the circle
    for (int i = 0; i < 5; i++)
    {
        // Calculating theta of the current iteratrion i
        float outerTheta = smath::TAU * ((float)i / 5.0f) - (smath::PI / 10.0f);
        float innerTheta = smath::TAU * ((float)i / 5.0f) + (smath::PI / 10.0f);

        // Outer Outer vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + (outerRadius + thicknessOffset) * cos(outerTheta), center.y + (outerRadius + thicknessOffset) * sin(outerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });

        // Inner Outer vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + (outerRadius - thicknessOffset) * cos(outerTheta), center.y + (outerRadius - thicknessOffset) * sin(outerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });

        // Outer Inner vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + (outerRadius + thicknessOffset) * 0.38f * cos(innerTheta), center.y + (outerRadius + thicknessOffset) * 0.38f * sin(innerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });

        // Inner Inner vertex
        tempMesh.vertices.push_back(
            {
                smath::vec3(center.x + (outerRadius - thicknessOffset) * 0.38f * cos(innerTheta), center.y + (outerRadius - thicknessOffset) * 0.38f * sin(innerTheta), mCurrentLayerValue + layerOffset), // Position
                color // Color
            });
    }

    // Creating the indices for the triangles
    for (int i = 0; i < 20; i++)
    {
        // Triangle
        tempMesh.indices.push_back(i);
        tempMesh.indices.push_back((i + 1) % 20);
        tempMesh.indices.push_back((i + 2) % 20);
    }

    // Adding the temp mesh
    addMesh(tempMesh);
}

// Functions for adding meshes to the PosColorTex buffer
void GraphicsRenderer::addTexQuad(const SDL_FRect& rectangle, const float& layerOffset, const std::string& textureKey, const Color& color, bool centered)
{
    // Calling the add texture quad function of the buffer
    mpUITextureBuffer->addTexQuad(
        smath::vec3(rectangle.x, rectangle.y, mCurrentLayerValue + layerOffset),
        smath::vec2(rectangle.w, rectangle.h),
        textureKey,
        color, centered
    );
}
void GraphicsRenderer::addTexQuad(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const std::string& textureKey, const Color& color, bool centered)
{
    // Calling the add texture quad function of the buffer
    mpUITextureBuffer->addTexQuad(smath::vec3(position, mCurrentLayerValue + layerOffset), size, textureKey, color, centered);
}

// Viewport function definitions
void GraphicsRenderer::setNewViewport(int x, int y, int w, int h)
{
    // Adding the viewport to the vector
    mViewports.push_back({ x, y, w, h });

    // Adding viewports to all the meshes
    mpUITextureBuffer->startNewViewport();
    mpPosColorBuffer->startNewViewport();
    mpTextBuffer->startNewViewport();
}

// Runtime Buffer Function Definitions
void GraphicsRenderer::updateDepthTexture(smath::ivec2 dimensions)
{
    // If the dimensions are the same, and the depth texture exists, do nothing
    if (mpDepthTexture && mDepthTextureDimensions == dimensions)
        return;

    // If there is a depth texture already, destroy it
    if (mpDepthTexture)
        SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpDepthTexture);

    // Creating the new depth texture
    SDL_GPUTextureCreateInfo depthTextureInfo =
    {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = (uint32_t)dimensions.x,
        .height = (uint32_t)dimensions.y,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
    };

    // Actually creating the texture
    mpDepthTexture = SDL_CreateGPUTexture(GraphicsDevice::getInstance()->getDevice(), &depthTextureInfo);
    if (!mpDepthTexture)
    {
        std::cout << "Failed to create depth texture with errors: " << SDL_GetError() << std::endl;
        return;
    }

    // Setting the texture's name
    SDL_SetGPUTextureName(GraphicsDevice::getInstance()->getDevice(), mpDepthTexture, "Renderer_DepthTexture");
}
void GraphicsRenderer::drawBuffers()
{
    // Making the GPU Device easier to type out
    SDL_GPUDevice* device = GraphicsDevice::getInstance()->getDevice();

    // Waiting for GPU ISL
    SDL_WaitForGPUIdle(device);

    // * ---------------------- *
    // |  Stage 1: Upload Data  |
    // * ---------------------- *

    // Acquiring the command buffer for uploading
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    if (commandBuffer == nullptr)
    {
        std::cout << "Failed to acquire upload command buffer with errors: " << SDL_GetError() << std::endl;
        return;
    }

    // Updating the buffers
    mpPosColorBuffer->sendMeshToGPU(commandBuffer);
    mpUITextureBuffer->sendMeshToGPU(commandBuffer);
    mpTextBuffer->sendMeshToGPU(commandBuffer);
    mpGameObjectBuffer->drawToTexture();


    // * -------------------- *
    // |  Stage 2: Rendering  |
    // * -------------------- *

    // Acquiring the swapchain texture
    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, GraphicsDevice::getInstance()->getWindow(), &swapchainTexture, nullptr, nullptr))
    {
        std::cout << "Failed to aquire swapchain texture with errors: " << SDL_GetError() << std::endl;
        SDL_CancelGPUCommandBuffer(commandBuffer);
        return;
    }

    // If swapchain properly acquired, run the actual rendering
    if (swapchainTexture)
    {
        // Setting color target info
        SDL_GPUColorTargetInfo colorTargetInfo =
        {
            .texture = swapchainTexture,
            .clear_color = { 0.15f, 0.15f, 0.15f, 1.0f },
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE
        };

        // Setting the depth stencil info
        SDL_GPUDepthStencilTargetInfo depthStencilInfo =
        {
            .texture = mpDepthTexture,
            .clear_depth = 0.0f,
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
            .cycle = false,
        };

        // Starting render pass
        SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, &depthStencilInfo);
        if (!renderPass)
        {
            std::cout << "Failed to begin the render pass with errors: " << SDL_GetError() << std::endl;
            return;
        }

        // Drawing each buffer to the render pass
        mpPosColorBuffer->drawBuffer(commandBuffer, renderPass);
        mpUITextureBuffer->drawBuffer(commandBuffer, renderPass);
        mpTextBuffer->drawBuffer(commandBuffer, renderPass);
        mpGameObjectBuffer->drawBuffer(commandBuffer, renderPass);

        // Ending the render pass and submitting the command buffer
        SDL_EndGPURenderPass(renderPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }

    // Clearing the buffers
    mpPosColorBuffer->clearMesh();
    mpUITextureBuffer->clearMesh();
    mpTextBuffer->clearMesh();

    // Clearing the viewports
    mViewports.clear();
}