#include "textmenu.h"

void handler_undefined() {
    printf("Undefined command\n");
}

// CommandStorage interface ====

Command commandCreate(char *name, char *description, void (*handler)(void*)) {
    Command result;
    strcpy(result.name, name);
    strcpy(result.description, description);
    result.handler = handler;
    return result;
}

CommandStorage *commandStorageCreate() {
    CommandStorage *result = malloc(sizeof(CommandStorage));
    result->size = 0;
    result->capacity = 1;
    result->commands = malloc(sizeof(Command));
    return result;
}

void commandStorageDestroy(CommandStorage *commandStorage) {
    assert(commandStorage != NULL);
    free(commandStorage->commands);
    free(commandStorage);
}

void commandStoragePush(CommandStorage *commandStorage, Command command) {
    assert(commandStorage != NULL);
    commandStorage->size++;
    if (commandStorage->size > commandStorage->capacity) {
        commandStorage->capacity *= 2;
        commandStorage->commands = realloc(commandStorage->commands, sizeof(Command) * commandStorage->capacity);
    }
    commandStorage->commands[commandStorage->size - 1] = command;
}

void commandStorageClear(CommandStorage *commandStorage) {
    assert(commandStorage != NULL);
    commandStorage->size = 0;
}

// \CommandStorage interface ===

// TextMenu interface ====

void textMenuDescript(CommandStorage *storage) {
    assert(storage != NULL);
    printf("You entered interactive mode. Here's the list of commands:\n");
    for (int i = 0;i < storage->size;i++) {
        printf("    %s\n", storage->commands[i].description);
    }
    printf("---------------\n    exit - exit program.\n");
}

void textMenuMatch(CommandStorage *storage, void *obj, char *name) {
    //assert(storage != NULL && obj != NULL);
    for (int i = 0;i < storage->size;i++) {
        if (strcmp(name, storage->commands[i].name) == 0) {
            (*storage->commands[i].handler)(obj);
            return;
        }
    }
    handler_undefined();
}

void textMenuStart(CommandStorage *storage, void *obj) {
    assert(storage != NULL);
    printf("Starting session\n");
    textMenuDescript(storage);
    char command[COMMAND_LENGTH];

    bool terminated = false;
    while (!terminated) {
        printf("> ");
        scanf("%s", command);
        // Exit checking
        if (strcmp("exit", command) == 0) {
            printf("Goodbye!\n");
            break;
        }
        textMenuMatch(storage, obj, command);
    }
}

// \TextMenu interface ===

// ===================== ||
// Macro Wrapper         ||
// ===================== ||

CommandStorage *__TEXTMENU_GLOBAL_STORAGE;

void textMenuFastInitialize(int commandCount, ...) {
    __TEXTMENU_GLOBAL_STORAGE = commandStorageCreate();

    va_list args;
    va_start(args, commandCount);

    for (int i = 0; i < commandCount; i++) {
        char *name = va_arg(args, char*);
        char *description = va_arg(args, char*);
        CommandHandler handler = va_arg(args, CommandHandler);
        Command command = commandCreate(name,
                                        description,
                                        handler);
        commandStoragePush(__TEXTMENU_GLOBAL_STORAGE, command);
    }

    va_end(args);
}

void textMenuFastStart(void *(*creator)(void), void (*destroyer)(void*)) {
    assert(__TEXTMENU_GLOBAL_STORAGE != NULL && (destroyer != NULL));
    void *obj = (*creator)();
    textMenuStart(__TEXTMENU_GLOBAL_STORAGE, obj);
    (*destroyer)(obj);
}

void textMenuFastClose() {
    commandStorageDestroy(__TEXTMENU_GLOBAL_STORAGE);
    __TEXTMENU_GLOBAL_STORAGE = NULL;
}
