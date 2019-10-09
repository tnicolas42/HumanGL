#include "humanGL.hpp"
#include <iostream>

/*
	called every frame
*/
void	processInput(GLFWwindow *window)
{
	tWinUser	*winU;
	float		crntFrame;

	winU = (tWinUser *)glfwGetWindowUserPointer(window);
	crntFrame = glfwGetTime();
	winU->dtTime = crntFrame - winU->lastFrame;
	winU->lastFrame = crntFrame;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        winU->cam->processKeyboard(CamMovement::Forward, winU->dtTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        winU->cam->processKeyboard(CamMovement::Backward, winU->dtTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        winU->cam->processKeyboard(CamMovement::Left, winU->dtTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        winU->cam->processKeyboard(CamMovement::Right, winU->dtTime);
}

void	keyCb(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void	scrollCb(GLFWwindow *window, double xOffset, double yOffset) {
	tWinUser		*winU;

	(void)xOffset;
	winU = (tWinUser *)glfwGetWindowUserPointer(window);
	winU->cam->processMouseScroll(yOffset);
}

void	mouseCb(GLFWwindow *window, double xPos, double yPos)
{
	tWinUser		*winU;
	static float	lastX = SCREEN_W / 2.0;
	static float	lastY = SCREEN_H / 2.0;
	static u_int8_t	firstTwoCall = 2;
	float			xOffset;
	float			yOffset;

	winU = (tWinUser *)glfwGetWindowUserPointer(window);

	if (firstTwoCall) {
		lastX = xPos;
		lastY = yPos;
		--firstTwoCall;
	}

	xOffset = xPos - lastX;
	// reversed since y-coordinates go from bottom to top
	yOffset = lastY - yPos;
	winU->cam->processMouseMovement(xOffset, yOffset);

	lastX = xPos;
	lastY = yPos;
}

void	frambuffResizeCb(GLFWwindow *window, int width, int height)
{
	tWinUser	*winU;

	winU = (tWinUser *)glfwGetWindowUserPointer(window);
	winU->width = width;
	winU->height = height;
	glViewport(0, 0, width, height);
}

bool	initWindow(GLFWwindow **window, const char *name, tWinUser *winU)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Could not start glfw3\n");
		return (false);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*window = glfwCreateWindow(SCREEN_W, SCREEN_H, name, NULL, NULL);
	if (!(*window))
	{
		fprintf(stderr, "Fail to create glfw3 window\n");
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(*window);

	glfwSetFramebufferSizeCallback(*window, frambuffResizeCb);
	glfwSetCursorPosCallback(*window, mouseCb);
	glfwSetKeyCallback(*window, keyCb);
	glfwSetScrollCallback(*window, scrollCb);

	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	glfwSetWindowUserPointer(*window, winU);
	return (true);
}
