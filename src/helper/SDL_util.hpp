#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>

class SDL {
    public:
        SDL(Uint32 flags);

        virtual ~SDL();

        // Disable copy contructor. SDL class cannot be copied
        SDL(const SDL& other) = delete;

        // Disable copy assignment. SDL class cannot be copied
        SDL& operator=(const SDL& other) = delete;

        // Disable move contructor. SDL class cannot be copied
        SDL(const SDL&& other) = delete;

        // Disable move assignment. SDL class cannot be copied
        SDL& operator=(const SDL&& other) = delete;
};
