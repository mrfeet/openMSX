#include "SDLGLVisibleSurface.hh"
#include "SDLGLOffScreenSurface.hh"
#include "GLContext.hh"
#include "GLSnow.hh"
#include "OSDConsoleRenderer.hh"
#include "OSDGUILayer.hh"
#include "Display.hh"
#include "RenderSettings.hh"
#include "CliComm.hh"
#include "PNG.hh"
#include "build-info.hh"
#include "MemBuffer.hh"
#include "outer.hh"
#include "vla.hh"
#include "InitException.hh"
#include "unreachable.hh"
#include <memory>

#include "GLUtil.hh"

namespace openmsx {

SDLGLVisibleSurface::SDLGLVisibleSurface(
		unsigned width, unsigned height,
		Display& display_,
		RTScheduler& rtScheduler_,
		EventDistributor& eventDistributor_,
		InputEventGenerator& inputEventGenerator_,
		CliComm& cliComm_,
		VideoSystem& videoSystem_)
	: SDLVisibleSurfaceBase(display_, rtScheduler_, eventDistributor_,
	                        inputEventGenerator_, cliComm_, videoSystem_)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	int flags = SDL_WINDOW_OPENGL;
	//flags |= SDL_RESIZABLE;
	createSurface(width, height, flags);

	// Create an OpenGL ES 2.0 profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	glContext = SDL_GL_CreateContext(window.get());
	if (!glContext) {
		throw InitException(
			"Failed to create openGL ES 2.0 context: ", SDL_GetError());
	}

	// The created surface may be larger than requested.
	// If that happens, center the area that we actually use.
	int w, h;
	SDL_GL_GetDrawableSize(window.get(), &w, &h);
	calculateViewPort(gl::ivec2(width, height), gl::ivec2(w, h));
	auto [vx, vy] = getViewOffset();
	auto [vw, vh] = getViewSize();
	glViewport(vx, vy, vw, vh);

	setOpenGlPixelFormat();
	gl::context = std::make_unique<gl::Context>(width, height);

	getDisplay().getRenderSettings().getVSyncSetting().attach(vSyncObserver);
	// set initial value
	vSyncObserver.update(getDisplay().getRenderSettings().getVSyncSetting());
}

SDLGLVisibleSurface::~SDLGLVisibleSurface()
{
	getDisplay().getRenderSettings().getVSyncSetting().detach(vSyncObserver);

	gl::context.reset();
	SDL_GL_DeleteContext(glContext);
}

void SDLGLVisibleSurface::saveScreenshot(const std::string& filename)
{
	saveScreenshotGL(*this, filename);
}

void SDLGLVisibleSurface::saveScreenshotGL(
	const OutputSurface& output, const std::string& filename)
{
	auto [x, y] = output.getViewOffset();
	auto [w, h] = output.getViewSize();

	// OpenGL ES only supports reading RGBA (not RGB)
	MemBuffer<uint8_t> buffer(w * h * 4);
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

	// perform in-place conversion of RGBA -> RGB
	VLA(const void*, rowPointers, h);
	for (int i = 0; i < h; ++i) {
		uint8_t* out = &buffer[w * 4 * i];
		const uint8_t* in = out;
		rowPointers[h - 1 - i] = out;

		for (int j = 0; j < w; ++j) {
			out[3 * j + 0] = in[4 * j + 0];
			out[3 * j + 1] = in[4 * j + 1];
			out[3 * j + 2] = in[4 * j + 2];
		}
	}

	PNG::save(w, h, rowPointers, filename);
}

void SDLGLVisibleSurface::finish()
{
	SDL_GL_SwapWindow(window.get());
}

std::unique_ptr<Layer> SDLGLVisibleSurface::createSnowLayer()
{
	return std::make_unique<GLSnow>(getDisplay());
}

std::unique_ptr<Layer> SDLGLVisibleSurface::createConsoleLayer(
		Reactor& reactor, CommandConsole& console)
{
	const bool openGL = true;
	auto [width, height] = getLogicalSize();
	return std::make_unique<OSDConsoleRenderer>(
		reactor, console, width, height, openGL);
}

std::unique_ptr<Layer> SDLGLVisibleSurface::createOSDGUILayer(OSDGUI& gui)
{
	return std::make_unique<GLOSDGUILayer>(gui);
}

std::unique_ptr<OutputSurface> SDLGLVisibleSurface::createOffScreenSurface()
{
	return std::make_unique<SDLGLOffScreenSurface>(*this);
}

void SDLGLVisibleSurface::VSyncObserver::update(const Setting& setting)
{
	auto& surface = OUTER(SDLGLVisibleSurface, vSyncObserver);
	auto& syncSetting = surface.getDisplay().getRenderSettings().getVSyncSetting();
	assert(&setting == &syncSetting); (void)setting;

	// for now, we assume that adaptive vsync is the best kind of vsync, so when
	// vsync is enabled, we attempt adaptive vsync.
	int interval = syncSetting.getBoolean() ? -1 : 0;

	if ((SDL_GL_SetSwapInterval(interval) < 0) && (interval == -1)) {
		// "Adaptive vsync" is not supported by all drivers. SDL
		// documentation suggests to fallback to "regular vsync" in
		// that case.
		SDL_GL_SetSwapInterval(1);
	}
}

} // namespace openmsx
