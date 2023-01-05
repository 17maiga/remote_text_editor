#ifndef REMOTE_TEXT_EDITOR_SERVER_H
#define REMOTE_TEXT_EDITOR_SERVER_H

/**
 * Main server execution loop.
 */
int srv_run(void);

void* client_connected();

#endif // REMOTE_TEXT_EDITOR_SERVER_H
