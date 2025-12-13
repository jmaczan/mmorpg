#include <SDL3/SDL.h>
#include <iostream>
#include <boost/asio.hpp>
#include <webgpu/wgpu.h>
#include "sdl3webgpu.h"

// requestAdapterSync from https://eliemichel.github.io/LearnWebGPU/getting-started/adapter-and-device/the-adapter.html
WGPUAdapter requestAdapterSync(WGPUInstance instance, WGPURequestAdapterOptions const *options)
{
    // A simple structure holding the local information shared with the
    // onAdapterRequestEnded callback.
    struct UserData
    {
        WGPUAdapter adapter = nullptr;
        bool requestEnded = false;
    } userData;

    // Callback called by wgpuInstanceRequestAdapter when the request returns
    // This is a C++ lambda function, but could be any function defined in the
    // global scope. It must be non-capturing (the brackets [] are empty) so
    // that it behaves like a regular C function pointer, which is what
    // wgpuInstanceRequestAdapter expects (WebGPU being a C API). The workaround
    // is to convey what we want to capture through the pUserData pointer,
    // provided as the last argument of wgpuInstanceRequestAdapter and received
    // by the callback as its last argument.
    auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void * /*userdata2*/)
    {
        UserData &userData = *reinterpret_cast<UserData *>(userdata1);
        if (status == WGPURequestAdapterStatus_Success)
        {
            userData.adapter = adapter;
        }
        else
        {
            // std::throw_with_nested(std::runtime_error("Couldn't create a WebGPU adapter"));
            std::cout << "no adapter" << message.data << std::endl;
        }
        userData.requestEnded = true;
    };

    WGPURequestAdapterCallbackInfo callback_info;
    callback_info.mode = WGPUCallbackMode_AllowProcessEvents;
    callback_info.callback = onAdapterRequestEnded;
    callback_info.userdata1 = &userData;
    WGPUFuture adapter_future = wgpuInstanceRequestAdapter(
        instance /* equivalent of navigator.gpu */,
        options,
        callback_info);

    assert(userData.requestEnded);

    return userData.adapter;
}

WGPUDevice requestDeviceSync(WGPUAdapter adapter, WGPUDeviceDescriptor const *descriptor)
{
    struct UserData
    {
        WGPUDevice device = nullptr;
        bool requestEnded = false;
    };
    UserData userData;

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void *userdata1, void * /*userdata2*/)
    {
        UserData &userData = *reinterpret_cast<UserData *>(userdata1);
        if (status == WGPURequestDeviceStatus_Success)
        {
            userData.device = device;
        }
        else
        {
            std::cout << "Could not get WebGPU device: " << message.data << std::endl;
        }
        userData.requestEnded = true;
    };

    WGPURequestDeviceCallbackInfo callback_info;
    callback_info.mode = WGPUCallbackMode_AllowProcessEvents;
    callback_info.callback = onDeviceRequestEnded;
    callback_info.userdata1 = &userData;

    WGPUFuture device_future = wgpuAdapterRequestDevice(
        adapter,
        descriptor,
        callback_info);

    assert(userData.requestEnded);

    return userData.device;
}

int main()
{
    WGPUInstanceDescriptor wgpu_desc = {};
    wgpu_desc.nextInChain = nullptr;
    WGPUInstance instance = wgpuCreateInstance(&wgpu_desc);
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("MMORPG", 800, 600, SDL_WINDOW_RESIZABLE);
    WGPUSurface surface = SDL_GetWGPUSurface(instance, window);
    WGPURequestAdapterOptions adapter_options = {};
    adapter_options.nextInChain = nullptr;
    WGPUAdapter adapter = requestAdapterSync(instance, &adapter_options);
    WGPUDeviceDescriptor device_desc = {};
    device_desc.nextInChain = nullptr;
    device_desc.requiredFeatureCount = 0;
    device_desc.requiredLimits = nullptr;
    WGPUStringView device_label = {};
    device_label.data = "Device";
    device_desc.label = device_label;
    WGPUStringView default_queue_label = {};
    default_queue_label.data = "Default queue";
    device_desc.defaultQueue.label = default_queue_label;
    device_desc.defaultQueue.nextInChain = nullptr;

    WGPUDevice device = requestDeviceSync(adapter, &device_desc);
    wgpuAdapterRelease(adapter);

    SDL_Event event;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }
    }

    wgpuInstanceRelease(instance);
    wgpuDeviceRelease(device);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
