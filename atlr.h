#ifndef ATLR_H
#define ATLR_H 1

/*
 * @author: carlos torija <atelier@torijacarlos.com>
 *
 * Atlr library: Personal library for personal tools :)
 *
 * WARNING: Might be dumb as f*ck, but I'm using the convention from python of
 * naming private things starting with a `_`. Don't be dumb and use them
 * directly... (°ー°; ) I'll also try to avoid been dumb, and try to have the 
 * least instances of those "private" things
 */

// ===========================================================
// @dependencies
// STUDY: Is there any possibility to remove some of this?
// ===========================================================

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// TODO: make stubs for stb functions

// ===========================================================
// @definition: symbols
// ===========================================================

#define ATLR_SEC_TO_US 1000000

#define ATLR_DEFAULT_STR_CAPACITY 50
#define ATLR_KILOBYTE 1024
#define ATLR_MEGABYTE (ATLR_KILOBYTE * 1024)
#define ATLR_GIGABYTE (ATLR_MEGABYTE * 1024)

#define ATLR_MAX_U64 18446744073709551615 - 1
#define ATLR_MAX_U32 4294967295 - 1

#define ATLR_PI 3.14159265
#define ATLR_RADIAN ATLR_PI / 180

#define ATLR_DECIMAL_DETAIL 1000000

// NOTE: dumb shorthands
#define loop while (1) 
#define atlr_isdigit(digit) ((digit - '0') >= 0 && (digit - '0') <= 9)

// NOTE: log constants
#define ATLR_LOG_H_SIZE 50
#define ATLR_LOG_SIZE  500
#ifdef ATLR_DEBUG
#define ATLR_ENV "dev"
#else
#define ATLR_ENV "release"
#endif

// NOTE: file constants
#define ATLR_DEFAULT_FILE_PERM 0644

// NOTE: type shorthands
typedef unsigned char  u8;
typedef signed char    s8;
typedef unsigned short u16;
typedef signed short   s16;
typedef unsigned int   u32;
typedef signed int     s32;
typedef unsigned long  u64;
typedef signed long    s64;
typedef float          f32;
typedef double         f64;
typedef unsigned int   b32;

#if defined __x86_64__
typedef unsigned long usize;
typedef signed long   ssize;
#else
typedef unsigned int usize;
typedef signed int   ssize;
#endif

// ===========================================================
// @header: algebra
// ===========================================================

typedef struct {
    union {
        struct {
            f64 x, y;
        };
        // NOTE: {0:independant, 1:dependant}
        f64 values[2];
    };
} Vec2;

typedef struct {
    f64 values[4];
} Matrix2x2;

typedef struct {
    Vec2* points;
    u32 count;
} Line;

typedef struct {
    Vec2 points[3];
} Triangle;

typedef struct {
    s32 x, y;
    s32 w, h;
} Rectangle;

static Matrix2x2 atlr_algebra_get_roll_m2x2(f64 degree);
static Vec2      atlr_algebra_cross_m2x2_v2(Matrix2x2 m, Vec2 v);
static Vec2      atlr_algebra_vec2_substract(Vec2 a, Vec2 b);
static b32       atlr_algebra_vec2_equal(Vec2 a, Vec2 b);

// ===========================================================
// @header: string
// ===========================================================

typedef struct {
    u32 len;
    u32 capacity;
    char *data;
} AtlrString;

static AtlrString atlr_str_create_empty_with_capacity(u32 capacity);
static AtlrString atlr_str_create_empty();
static AtlrString atlr_str_create_from(char *str);
static AtlrString atlr_str_get_left_side_from_pos(AtlrString *str, u64 split_pos);
static AtlrString atlr_str_get_right_side_from_pos(AtlrString *str, u64 split_pos);
static void       atlr_str_clear(AtlrString *string);
static void       atlr_str_to_lower(AtlrString *str);
static void       atlr_str_increase_capacity(AtlrString *str);
static void       atlr_str_concat_atlr_string(AtlrString *dest, AtlrString *src);
static void       atlr_str_concat_string(AtlrString *str, char *data, u32 len);
static void       atlr_str_concat_char(AtlrString *str, char data);
static s64        atlr_str_find_char_last_pos(AtlrString *str, char c);

// ===========================================================
// @header: results
// ===========================================================

typedef enum {
    // ¯\_(ツ)_/¯
    ATLR_ERROR_UNKNOWN,
    // atlr_fs errors
    ATLR_ERROR_FS_NOT_INITIALIZED,
    ATLR_ERROR_FS_PATH_DOES_NOT_EXIST,
    ATLR_ERROR_FS_FAILED_CREATING_DIR,
    ATLR_ERROR_FS_TRIED_LOADING_DIR,
    ATLR_ERROR_FS_COULD_NOT_OPEN_FILE,
    ATLR_ERROR_FS_COULD_NOT_SAVE_FILE,
    ATLR_ERROR_FS_COULD_NOT_CREATE_FILE,
} AtlrError;

typedef struct {
    void *data;
    b32 is_success;
    AtlrError error;
} AtlrResult;

static char* atlr_get_error_message(AtlrResult r);

// ===========================================================
// @header: datetime
// ===========================================================

typedef struct tm Time;
typedef struct timeval TimeVal;

static u64 atlr_dt_get_time();
static AtlrString atlr_dt_get_current_time_str();

// ===========================================================
// @header: logger
// ===========================================================

static FILE *_ATLR_LOG_FILE = NULL;

static void  _atlr_log_init();
static void  atlr_log_set_output(char*);

#define atlr_log(lvl, ...) {                      \
    _atlr_log_init();                             \
    fprintf(_ATLR_LOG_FILE, "[%s]:[%s]:[%s:%d]:", \
        atlr_dt_get_current_time_str().data, lvl, \
        __FILE__, __LINE__);                      \
    fprintf(_ATLR_LOG_FILE, "[%s]:", ATLR_ENV);   \
    fprintf(_ATLR_LOG_FILE,## __VA_ARGS__);       \
    fprintf(_ATLR_LOG_FILE, "\n");                \
}

#ifdef ATLR_DEBUG
#define atlr_log_debug(...) atlr_log("DEBUG",## __VA_ARGS__)
#else
#define atlr_log_debug(...)
#endif
#define atlr_log_info(...) atlr_log("INFO",## __VA_ARGS__)
#define atlr_log_warning(...) atlr_log("WARNING",## __VA_ARGS__)
#define atlr_log_error(...) atlr_log("ERROR",## __VA_ARGS__)

// ===========================================================
// @header: random
// ===========================================================

static void _atlr_init_random();
static f64  atlr_random_f64(f64 upper_limit, f64 lower_limit);
static s64  atlr_random_s64(s64 upper_limit, s64 lower_limit);
static s32  atlr_random_s32(s32 upper_limit, s32 lower_limit);
static u64  atlr_random_u64(u64 upper_limit, u64 lower_limit);
static u32  atlr_random_u32(u32 upper_limit, u32 lower_limit);

// ===========================================================
// @header: memory arenas
// ===========================================================

typedef struct {
    void* data;
    u64 capacity;
    u64 used;
} AtlrArena;

static AtlrArena atlr_mem_create_arena(u64 capacity);
static u8*       atlr_mem_allocate(AtlrArena* arena, u64 size);
static void      atlr_mem_clear(AtlrArena* arena, char* id);
static void      atlr_mem_free(AtlrArena* arena, char* id);


// ===========================================================
// @header: atlr_fs
// ===========================================================

typedef struct stat AtlrStat;
typedef struct dirent DirEnt;

typedef struct {
    AtlrString path;
    AtlrString parent;
    AtlrString filename;
    AtlrString format;
    u64 size;
    b32 is_directory;
    b32 is_loaded;
    void *data;
} AtlrFile;

typedef struct {
    AtlrString path;
    AtlrFile *files;
    u32 count;
    u32 capacity;
} AtlrDirectory;

static AtlrFile      *atlr_fs_get_file(char *filepath);
static AtlrResult     atlr_fs_create_file(char *filepath);
static AtlrResult     atlr_fs_load_file(AtlrFile *file);
static void           atlr_fs_unload_file(AtlrFile *file);
static AtlrResult     atlr_fs_save_file(AtlrFile *src);
static AtlrResult     atlr_fs_copy_file(AtlrFile *src, char *dest);
static AtlrResult     atlr_fs_create_directory(char *pathname);
static AtlrDirectory  atlr_fs_get_directory(char *pathname);
static void           atlr_fs_copy_dir(AtlrDirectory *directory, AtlrString* tmp_dir);
static void           atlr_remove_dir_files(AtlrString *dir_name, b32 remove_dir);

// ===========================================================
// @header: csv parser
// ===========================================================

typedef struct {
    AtlrString key; 
    AtlrString value; 
} AtlrCsvColumn;

typedef struct {
   AtlrCsvColumn *cols;
   u32 col_count;
} AtlrCsvRow;

typedef struct {
   AtlrCsvRow *rows;
   u32 row_count;
} AtlrCsv;

static AtlrCsv atlr_csv_load(char *csv_contents, AtlrArena *mem);

// ===========================================================
// @header: json parser
// ===========================================================

// STUDY: is this renaming a good idea?
typedef struct stat FileStat;

typedef enum {
    ATLR_JSON_VALUE_TYPE_UNKNOWN = 0,
    ATLR_JSON_VALUE_TYPE_OBJECT = 1,
    ATLR_JSON_VALUE_TYPE_ARRAY = 2,
    ATLR_JSON_VALUE_TYPE_STRING = 3,
    ATLR_JSON_VALUE_TYPE_NUMBER = 4,
    ATLR_JSON_VALUE_TYPE_BOOLEAN = 5,
    ATLR_JSON_VALUE_TYPE_NULL = 6,
} AtlrJsonValueType;

typedef struct {
    void *data;
    AtlrJsonValueType type;
} AtlrJsonValue;

typedef struct {
    u64 len;
    AtlrJsonValue *values;
} AtlrJsonArray;

typedef struct {
    char *key;
    AtlrJsonValue *value;
} AtlrJsonKeyValuePair;

typedef struct {
    u64 len;
    AtlrJsonKeyValuePair *pairs;
} AtlrJsonObject;
  
// NOTE: tokens taken from json.org
typedef enum {
    ATLR_JSON_TOKEN_TYPE_UNKNOWN = 0, // no data
    ATLR_JSON_TOKEN_TYPE_LEFT_BRACE = 1, // no data
    ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET = 2, // no data
    ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE = 3, // no data
    ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET = 4, // no data
    ATLR_JSON_TOKEN_TYPE_STRING = 5,
    ATLR_JSON_TOKEN_TYPE_NUMBER = 6,
    ATLR_JSON_TOKEN_TYPE_BOOLEAN_TRUE = 7,
    ATLR_JSON_TOKEN_TYPE_BOOLEAN_FALSE = 8,
    ATLR_JSON_TOKEN_TYPE_COLON = 9, // no data
    ATLR_JSON_TOKEN_TYPE_COMMA = 10, // no data
    ATLR_JSON_TOKEN_TYPE_NULL = 11, // no data
} AtlrJsonTokenType;

typedef struct {
    void *data;
    AtlrJsonTokenType type;
} AtlrJsonToken;
static u64             _atlr_tokenize_string(char *contents, s64 contents_size, AtlrJsonToken *tokens);
static AtlrJsonValue   _atlr_parse_value(AtlrJsonToken **tokens);
static AtlrJsonArray*  _atlr_make_array(AtlrJsonToken *cursor);
static AtlrJsonArray*  _atlr_parse_array(AtlrJsonToken **tokens);
static AtlrJsonObject* _atlr_make_object(AtlrJsonToken *cursor);
static AtlrJsonObject* _atlr_parse_object(AtlrJsonToken **tokens);
static char*           atlr_json_token_type_to_string(AtlrJsonTokenType type);
static char*           atlr_json_value_type_to_string(AtlrJsonValueType type);
static AtlrJsonValue   atlr_get_json_from_file(char*);
static AtlrJsonValue*  atlr_get_value_by_key(AtlrJsonObject *object, char* key);

// ===========================================================
// @header: atlr_image
// ===========================================================

typedef struct {
    u32* data;
    s32 width;
    s32 height;
    s32 channels;
} AtlrImage;

static AtlrImage atlr_image_load(char*);
static u32       atlr_image_get_color(AtlrImage, s32, s32);

// ===========================================================
// @header: atlr_font
// ===========================================================

typedef struct {
    char codepoint;
    s32 width;
    s32 height;
    s32 x_shift;
    s32 y_shift;
    u8* bitmap;
} AtlrFontGlyph;

typedef struct {
    AtlrFontGlyph* glyphs;
    u64 last;
    u64 capacity;
} AtlrFontAtlas;

typedef struct {
   stbtt_fontinfo font;
   AtlrFile* font_file;
   f32 scale;
   f32 pixel_height;
   AtlrFontAtlas atlas;
} AtlrFont;

static AtlrFont atlr_font_load(char* font_path, f32 font_scale, AtlrArena* arena);
static AtlrFontGlyph atlr_font_get_glyph(AtlrFont* font, char codepoint);

// ===========================================================
// @header: atlr_rtzr
// ===========================================================

static Line atlr_rtzr_interpolate(s32 dep_end, s32 dep_start, s32 ind_end, s32 ind_start);
static void atlr_rtzr_draw_pixel(u32* data, s32 w, s32 h, s32 x, s32 y, u32 color);
static void atlr_rtzr_draw_line(u32* data, s32 w, s32 h, Vec2 from, Vec2 to, u32 color);
static void atlr_rtzr_order_triangle(Triangle *triangle);
static void atlr_rtzr_draw_triangle(u32* data, s32 w, s32 h, Triangle triangle, u32 color);
static void atlr_rtzr_draw_filled_triangle(u32* data, s32 w, s32 h, Triangle triangle, u32 color);
static void atlr_rtzr_make_rect_triangles(Rectangle rect, Triangle* triangles);
static void atlr_rtzr_draw_filled_rectangle(u32* data, s32 w, s32 h, Rectangle rect, u32 color, Matrix2x2 rotation);
static void atlr_rtzr_draw_triangle_texture(u32* data, s32 w, s32 h, Triangle, AtlrImage, u32, u32, b32, Matrix2x2);
static void atlr_rtzr_draw_image(u32* data, s32 w, s32 h, AtlrImage image, Rectangle dest, Matrix2x2 rotation);

// ===========================================================
// @header: math
// ===========================================================

static f64 atlr_string_to_f64(char* str, u64 len);

#ifndef ATLR_HEADER_ONLY

// ===========================================================
// @implementation: string functions
// ===========================================================

static AtlrString atlr_str_create_empty_with_capacity(u32 capacity) {
    AtlrString str = {};
    str.len = 0;
    str.capacity = capacity;
    str.data = (char*) malloc(sizeof(char) * str.capacity);
    memset(str.data, 0, str.capacity);
    return str;
}

static AtlrString atlr_str_create_empty() {
    return atlr_str_create_empty_with_capacity(ATLR_DEFAULT_STR_CAPACITY);
}

static AtlrString atlr_str_create_from(char *str) {
    u64 len = strlen(str);
    AtlrString _str = atlr_str_create_empty_with_capacity(len * 2);
    for (u32 i = 0; i < len; i++) {
        _str.data[i] = str[i];
    }
    _str.len = len;
    return _str;
}

static void atlr_str_increase_capacity(AtlrString *str) {
    str->capacity *= 2;
    char *new_data = (char*) malloc(sizeof(char) * str->capacity);
    memset(new_data, 0, str->capacity);
    for (u32 i = 0; i <= str->len; i++) {
        new_data[i] = str->data[i];
    }
    free(str->data);
    str->data = new_data;
}

static void atlr_str_concat_atlr_string(AtlrString *dest, AtlrString *src) {
    atlr_str_concat_string(dest, src->data, src->len);
}

static void atlr_str_concat_string(AtlrString *str, char *data, u32 len) {
    while ((len + str->len) >= str->capacity) {
        atlr_str_increase_capacity(str);
    }
    for (u32 i = 0; i < len; i++) {
        str->data[str->len + i] = data[i];
    }
    str->len += len;
}

static void atlr_str_concat_char(AtlrString *str, char data) {
    while ((1 + str->len) >= str->capacity) {
        atlr_str_increase_capacity(str);
    }
    str->data[str->len] = data;
    str->len += 1;
}

static void atlr_str_clear(AtlrString *string) {
    string->len = 0;
    memset(string->data, 0, string->capacity);
}

static void atlr_str_to_lower(AtlrString *str) {
    for (u32 i = 0; i < str->len; i++) {
        str->data[i] = tolower(str->data[i]);
    }
}

static s64 atlr_str_find_char_last_pos(AtlrString *str, char c) {
    for (s64 i = str->len; i >= 0; i--) {
        if (str->data[i] == c) {
            return i;
        }
    }
    return -1;
}

static AtlrString atlr_str_get_left_side_from_pos(AtlrString *str, u64 split_pos) {
    AtlrString left_side = atlr_str_create_empty();
    for (u64 i = 0; i < split_pos; i++) {
        atlr_str_concat_char(&left_side, str->data[i]);
    }
    return left_side;
}

static AtlrString atlr_str_get_right_side_from_pos(AtlrString *str, u64 split_pos) {
    AtlrString right_side = atlr_str_create_empty();
    for (u64 i = split_pos; i < str->len; i++) {
        atlr_str_concat_char(&right_side, str->data[i]);
    }
    return right_side;
}

static AtlrString atlr_str_left_pad(char *start_str, u16 width, char pad_char) {
    AtlrString str = atlr_str_create_empty();
    atlr_str_concat_string(&str, start_str, strlen(start_str));
    if (str.len > width) {
        return str;
    }
    while (width > str.capacity) {
        atlr_str_increase_capacity(&str);
    }
    // WARNING: This is so dumb
    // sprintf string formatting has a way to do this out of the box, move to
    // that, no need to implement left_pad
    // STUDY: is sprintf cross-platform?
    while (str.len < width) {
        char tmp_file_number[10];
        sprintf(tmp_file_number, "%c%s", pad_char, str.data);
        strcpy(str.data, tmp_file_number);
        str.len++;
    }
    return str;
}

// ===========================================================
// @implementation: datetime functions
// TODO: Consider moving to SDLs functions
// ===========================================================

AtlrString _atlr_date_str;

static u64 atlr_dt_get_time() {
    TimeVal time = {};
    gettimeofday(&time, NULL);
    return (ATLR_SEC_TO_US * time.tv_sec) + time.tv_usec;
}

static AtlrString atlr_dt_get_current_time_str() {
    // NOTE: ISO-8601
    if (_atlr_date_str.capacity == 0) {
        _atlr_date_str = atlr_str_create_empty_with_capacity(50);
    } else {
         atlr_str_clear(&_atlr_date_str);
    }
    time_t now = time(NULL);
    Time *t = localtime(&now);
    strftime(_atlr_date_str.data, _atlr_date_str.capacity - 1, "%FT%T%z", t);
    _atlr_date_str.len = 24;
    return _atlr_date_str;
}

// ===========================================================
// @implementation: atlr_logger
// ===========================================================

static void _atlr_log_init() {
    if (_ATLR_LOG_FILE == NULL) {
        _ATLR_LOG_FILE = stdout;
    }
}

static void atlr_log_set_output(char* output) {
    if (_ATLR_LOG_FILE != NULL && _ATLR_LOG_FILE != stdout) {
        fclose(_ATLR_LOG_FILE);
    }
    _ATLR_LOG_FILE = fopen(output, "a");
    atlr_log_debug("set log output to %s", output);
}

// ===========================================================
// @implementation: results
// ===========================================================

static char* atlr_get_error_message(AtlrResult r) {
    // NOTE: do not include \n!
    switch (r.error) {
        case ATLR_ERROR_FS_NOT_INITIALIZED: {
            return (char*) "the file has not been initialized";
        } break;
        case ATLR_ERROR_FS_PATH_DOES_NOT_EXIST: {
            return (char*) "the provided path does not exist";
        } break;
        case ATLR_ERROR_FS_FAILED_CREATING_DIR: {
            return (char*) "could not create directory";
        } break;
        case ATLR_ERROR_FS_TRIED_LOADING_DIR: {
            return (char*) "could not load directory as file";
        } break;
        case ATLR_ERROR_FS_COULD_NOT_OPEN_FILE: {
            return (char*) "error while opening file";
        } break;
        case ATLR_ERROR_FS_COULD_NOT_SAVE_FILE: {
            return (char*) "error while saving file";
        } break;
        case ATLR_ERROR_FS_COULD_NOT_CREATE_FILE: {
            return (char*) "error while creating file";
        } break;
        default: {
            return (char*) "unknown error";
        } break;
    }
}

// ===========================================================
// @implementation: random
// NOTE: functions are inclusive of the limits
// ===========================================================

static b32 _atlr_random_initialized = 0;

static void _atlr_init_random() {
    if (_atlr_random_initialized) {
        return;
    }
    u32 fd = open("/dev/urandom", O_RDONLY);
    u32 seed = 0;
    if (fd > 0) {
        read(fd, &seed, 4);
    } else {
        atlr_log_error("failed to open /dev/urandom");
        seed = 0;
    }
    srand(seed);
    close(fd);
    _atlr_random_initialized = 1;
}

static u32 atlr_random_u32(u32 upper_limit, u32 lower_limit) {
    if (!_atlr_random_initialized) {
        _atlr_init_random();
    }
    u32 range = upper_limit - lower_limit + 1;
    u32 rand_value = (u32) rand();
    return (rand_value % range) + lower_limit;
}

static u64 atlr_random_u64(u64 upper_limit, u64 lower_limit) {
    if (!_atlr_random_initialized) {
        _atlr_init_random();
    }
    u64 rand_value = (((u64) rand()) << 32) | ((u64) rand());
    u64 range = upper_limit - lower_limit + 1;
    return (rand_value % range) + lower_limit;
}

static s32 atlr_random_s32(s32 upper_limit, s32 lower_limit) {
    if (!_atlr_random_initialized) {
        _atlr_init_random();
    }
    s32 rand_value = ((s32) rand());
    s32 range = upper_limit - lower_limit + 1;
    return (rand_value % range) + lower_limit;
}

static s64 atlr_random_s64(s64 upper_limit, s64 lower_limit) {
    if (!_atlr_random_initialized) {
        _atlr_init_random();
    }
    s64 rand_value = (((u64) rand()) << 32) | ((u64) rand());
    s32 range = upper_limit - lower_limit + 1;
    return (rand_value % range) + lower_limit;
}

static f64 atlr_random_f64(f64 upper_limit, f64 lower_limit) {
    if (!_atlr_random_initialized) {
        _atlr_init_random();
    }
    s64 upper_limit_int = (s64) (upper_limit * ATLR_DECIMAL_DETAIL);
    s64 lower_limit_int = (s64) (lower_limit * ATLR_DECIMAL_DETAIL);
    s64 rand_value = atlr_random_s64(upper_limit_int, lower_limit_int);
    return (f64) ((f64)rand_value / ATLR_DECIMAL_DETAIL);
}

// ===========================================================
// @implementation: memory arena functions
// ===========================================================

static AtlrArena atlr_mem_create_arena(u64 capacity) {
    AtlrArena arena = (AtlrArena) {
        .data = malloc(capacity),
        .capacity = capacity,
        .used = 0,
    };
    return arena;
}

static u8* atlr_mem_allocate(AtlrArena* arena, u64 size) {
    if (arena->used + size > arena->capacity) {
        atlr_log_error("insufficient capacity");
        return NULL;
    }
    void* current = (u8*)arena->data + arena->used;
    memset(current, 0, size);
    arena->used += size;
    return current;
}

static AtlrArena atlr_mem_slice(AtlrArena* arena, u64 size) {
    void* arena_start = atlr_mem_allocate(arena, size);
    return (AtlrArena) {
        .data = arena_start,
        .capacity = size,
        .used = 0,
    };
}

static void atlr_mem_clear(AtlrArena* arena, char* id) {
    atlr_log_info("freeing mem arena (%s): used: %0.5f %%", id, ((f64) arena->used / arena->capacity) * 100.0);
    arena->used = 0;
}

static void atlr_mem_free(AtlrArena* arena, char* id) {
    atlr_log_info("freeing mem arena (%s): used: %0.5f %%", id, ((f64) arena->used / arena->capacity) * 100.0);
    free(arena->data);
    arena->used = 0;
}

// ===========================================================
// @implementation: atlr_fs
// ===========================================================

static void atlr_fs_build_file(AtlrFile *file) {
    if (file->path.len == 0) {
        atlr_log_error("Missing file's path");
        return;
    }
    file->size = 0;
    file->is_directory = 0;
    file->is_loaded = 0;
    // WARNING(torija): platform dependant
    s64 dir_pos = atlr_str_find_char_last_pos(&file->path, '/');
    s64 format_pos = atlr_str_find_char_last_pos(&file->path, '.');
    file->parent = atlr_str_get_left_side_from_pos(&file->path, dir_pos + 1);;
    file->filename = atlr_str_get_right_side_from_pos(&file->path, dir_pos + 1);;
    if (format_pos < 0) {
        file->format = atlr_str_create_empty();
    } else {
        file->format = atlr_str_get_right_side_from_pos(&file->path, format_pos + 1);;
        atlr_str_to_lower(&file->format);
    }
    AtlrStat sys_stat = {0};
    if (stat(file->path.data, &sys_stat) == 0) {
        file->is_directory = S_ISDIR(sys_stat.st_mode);
        file->size = sys_stat.st_size;
    } else {
        atlr_log_warning("%s:%s", "atlr_fs_make_file: path does not exists", file->path.data);
    }
}

static AtlrFile *atlr_fs_get_file(char *filepath) {
    AtlrFile *file = (AtlrFile *) malloc(sizeof(AtlrFile));
    file->path = atlr_str_create_from(filepath);
    atlr_fs_build_file(file);
    return file;
}

static AtlrResult atlr_fs_load_file(AtlrFile *file) {
    if (file->is_directory) {
        return (AtlrResult) { 
            .is_success = 0, 
            .error = ATLR_ERROR_FS_TRIED_LOADING_DIR, 
        };
    }
    if (file->path.len == 0 || file->size == 0) {
        return (AtlrResult) { 
            .is_success = 0, 
            .error = ATLR_ERROR_FS_NOT_INITIALIZED
        };
    }

    if (!file->is_loaded) {
        s64 fd = open(file->path.data, O_RDONLY | O_CREAT, ATLR_DEFAULT_FILE_PERM);
        if (fd <= 0) {
            return (AtlrResult) { 
                .is_success = 0, 
                .error = ATLR_ERROR_FS_COULD_NOT_OPEN_FILE
            };
        }

        file->data = malloc(file->size);
        read(fd, file->data, file->size);
        file->is_loaded = 1;
        close(fd);
    }
    return (AtlrResult) { .is_success = 1, };
}

static void atlr_fs_unload_file(AtlrFile *file) {
    if (!file->is_loaded) {
        return;
    }
    free(file->data);
    file->is_loaded = 0;
}

static AtlrResult atlr_fs_save_file(AtlrFile *src) {
    u64 fd = open(src->path.data, O_WRONLY | O_TRUNC);
    if (fd <= 0) {
        return (AtlrResult) { 
            .is_success = 0,
            .error = ATLR_ERROR_FS_COULD_NOT_SAVE_FILE
        };
    }
    write(fd, src->data, src->size);
    close(fd);
    return (AtlrResult) { .is_success = 1, };
}

static AtlrResult atlr_fs_copy_file(AtlrFile *src, char *dest) {
    AtlrResult load_res = atlr_fs_load_file(src);
    if (!load_res.is_success) {
        return load_res;
    }

    u64 fd = creat(dest, 0644);
    if (fd <= 0) {
        return (AtlrResult) { 
            .is_success = 0,
            .error = ATLR_ERROR_FS_COULD_NOT_CREATE_FILE
        };
    }
    write(fd, src->data, src->size);
    close(fd);
    atlr_fs_unload_file(src);
    return (AtlrResult) { .is_success = 1, };
}

static AtlrResult atlr_fs_create_directory(char *pathname) {
    AtlrResult r = (AtlrResult) {
        .is_success = 1,
    };
    AtlrStat sys_stat = {0};
    if (stat(pathname, &sys_stat) == -1 && mkdir(pathname, 0744) != 0) {
        atlr_log_error("Failed creating: %s (errno:%d)", pathname, errno);
        r.is_success = 0;
        r.error = ATLR_ERROR_FS_FAILED_CREATING_DIR;
    }
    return r;
}

static AtlrDirectory atlr_fs_get_directory(char *pathname) {
    AtlrDirectory atlr_dir = {};
    AtlrString dir_path = atlr_str_create_from(pathname);
    if (dir_path.data[dir_path.len - 1] != '/') {
        atlr_str_concat_char(&dir_path, '/');
    }
    atlr_dir.capacity = 64;
    atlr_dir.files = (AtlrFile *) malloc(sizeof(AtlrFile) * atlr_dir.capacity);
    atlr_dir.path = dir_path;
    atlr_dir.count = 0;

    DIR *directory = opendir(atlr_dir.path.data);
    u32 file_pathname_len = strlen(atlr_dir.path.data);
    while (directory) {
        DirEnt *entry = readdir(directory);
        // NOTE(torija): `entry->d_type == 4` if its a directory
        if (!entry) {
            break;
        }

        if (entry->d_type == 8) {
            AtlrString filename = atlr_str_create_empty_with_capacity(60);

            atlr_str_concat_atlr_string(&filename, &dir_path);
            if (filename.data[filename.len - 1] != '/') {
                atlr_str_concat_char(&filename, '/');
            }
            atlr_str_concat_string(&filename, entry->d_name, strlen(entry->d_name));

            if (atlr_dir.count >= atlr_dir.capacity) {
                atlr_dir.capacity *= 2;
                atlr_dir.files = (AtlrFile *) realloc(atlr_dir.files, sizeof(AtlrFile) * atlr_dir.capacity);
            }
            AtlrFile *file = (atlr_dir.files + atlr_dir.count); 
            memset(file, 0, sizeof(AtlrFile));
            file->path = filename;
            atlr_fs_build_file(file);
            atlr_dir.count++;
        } else if (errno != 0) {
            // TODO(torija): handle error
        }
    }
    return atlr_dir;
}

static void atlr_fs_copy_dir(AtlrDirectory *directory, AtlrString* dest_dir) {
    AtlrResult result = atlr_fs_create_directory(dest_dir->data);
    if (!result.is_success) {
        atlr_log_error("atlr_fs_copy_dir: Could not create dest directory");
    }
    AtlrString dest_file_name = atlr_str_create_empty_with_capacity(60);
    for (u32 i = 0; i < directory->count; i++) {
        AtlrFile file = directory->files[i];
        atlr_str_clear(&dest_file_name);
        atlr_str_concat_atlr_string(&dest_file_name, dest_dir);
        atlr_str_concat_atlr_string(&dest_file_name, &file.filename);
        atlr_fs_copy_file(&file, dest_file_name.data);
    }
}

static void atlr_remove_dir_files(AtlrString *dir_name, b32 remove_dir) {
    AtlrDirectory dir = atlr_fs_get_directory(dir_name->data);
    for (u16 i = 0; i < dir.count; i++) {
        AtlrFile file = dir.files[i];
        remove(file.path.data);
    }
    if (remove_dir) {
        remove(dir_name->data);
    }
}

// ===========================================================
// @implementation: atlr_csv
// ===========================================================

static AtlrCsv atlr_csv_load(char *csv_contents, AtlrArena *mem) {
    u32 current_column = 0;
    u32 current_row = 0;
    u32 total_columns = 0;
    AtlrString current = atlr_str_create_empty();

    u64 header_fields = 0;
    u64 reading_header = 1;
    u64 row_count = 0;
    for (u32 i = 0; i < strlen(csv_contents); i++) {
        switch (csv_contents[i]) {
            case '\n': {
                if (reading_header) {
                    header_fields++;
                    reading_header = 0;
                } else {
                    row_count++;
                }
            } break;
            case ',': {
                if (reading_header) {
                    header_fields++;
                }
            } break;
        }
    }

    AtlrCsv csv = {
        .rows = (AtlrCsvRow*) atlr_mem_allocate(mem, sizeof(AtlrCsvRow) * row_count),
        .row_count = 0,
    };
    AtlrString *header = (AtlrString*) atlr_mem_allocate(mem, sizeof(AtlrString) * header_fields);


    b32 is_header = 1;
    for (u32 i = 0; i < strlen(csv_contents); i++) {

        switch (csv_contents[i]) {
            case '\n': {
                if (is_header) {
                    is_header = 0;
                    header[current_column] = current;
                } else {
                    csv.rows[current_row].cols[current_column].key = header[current_column];
                    csv.rows[current_row].cols[current_column].value = current;
                    current_row++;
                }

                current_column = 0;
                current = atlr_str_create_empty();
                csv.rows[current_row].cols = (AtlrCsvColumn*) atlr_mem_allocate(mem, sizeof(AtlrCsvColumn) * header_fields);
                csv.rows[current_row].col_count = header_fields;
            } break;

            case ',': {
                if (is_header) {
                    header[current_column] = current;
                } else {
                    csv.rows[current_row].cols[current_column].key = header[current_column];
                    csv.rows[current_row].cols[current_column].value = current;
                }
                current_column++;
                current = atlr_str_create_empty();
            } break;

            default: {
                atlr_str_concat_char(&current, csv_contents[i]);
            } break;
        }

    }
    csv.row_count = current_row;
    return csv;
}

// ===========================================================
// @implementation: atlr_json
// ===========================================================

static u64 _atlr_tokenize_string(char *contents, s64 contents_size, AtlrJsonToken *tokens) {
    atlr_profile_start_with_id((char*) __func__, contents_size);
    u64 current_token = 0;
    for (s64 i = 0; i < contents_size; i++) {
        switch (contents[i]) {
            case '{': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_LEFT_BRACE;
            } break;
            case '}': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE;
            } break;
            case '[': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET;
            } break;
            case ']': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET;
            } break;
            case ',': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_COMMA;
            } break;
            case ':': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_COLON;
            } break;
            case 't': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_BOOLEAN_TRUE;
                i += 3;
            } break;
            case 'f': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_BOOLEAN_FALSE;
                i += 4;
            } break;
            case 'n': {
                tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_NULL;
                i += 3;
            } break;

            case '"': {
                s64 start_position = i;
                u32 size_string = 0;
                i++;
                u32 escape_seq = 0;
                while (contents[i] != '"') {
                    if (contents[i] == '\\') {
                        escape_seq++;
                        size_string++;
                        i++;
                    }
                    size_string++;
                    i++;
                }
                size_string += 2;

                atlr_profile_start_with_id((char*) "tokenize string", sizeof(char) * size_string + 1);
                char *value = (char*) malloc(sizeof(char) * size_string + 1);
                // TODO(torija): handle escape sequences
                for (u32 j = 0; j < size_string; j++) {
                    value[j] = contents[start_position + j];
                }
                value[size_string] = '\0';
                tokens[current_token].type = ATLR_JSON_TOKEN_TYPE_STRING;
                tokens[current_token].data = value;
                current_token++;
                atlr_profile_end();
            } break;

            case ' ':
            case '\n':
            case '\r':
            case '\t': {
                continue;
            } break;

            default: {
                if (contents[i] == '-' || atlr_isdigit(contents[i])) {
                    atlr_profile_start_with_id((char*) "tokenize number", 0);
                    s64 start_position = i;
                    u32 size_string = 0;
                    while (contents[i] == '-' || atlr_isdigit(contents[i]) || contents[i] == '.') {
                        size_string++;
                        i++;
                    }
                    i--;
                    atlr_profile_start_with_id((char*) "value", sizeof(char) * size_string);
                    char *value = (char *) malloc(sizeof(char) * size_string);
                    for (u32 j = 0; j < size_string; j++) {
                        value[j] = contents[start_position + j];
                    }
                    atlr_profile_end();
                    tokens[current_token].type = ATLR_JSON_TOKEN_TYPE_NUMBER;
                    atlr_profile_start_with_id((char*) "to float", sizeof(f64));
                    f64 *value_float = (f64 *) malloc(sizeof(f64));
                    *value_float = atlr_string_to_f64(value, size_string);
                    atlr_profile_end();
                    tokens[current_token].data = value_float;
                    free(value);
                    current_token++;
                    atlr_profile_end();
                } else {
                    tokens[current_token++].type = ATLR_JSON_TOKEN_TYPE_UNKNOWN;
                }
            } break;

        }
    }
    atlr_profile_end();
    return current_token;
}

static AtlrJsonValue _atlr_parse_value(AtlrJsonToken **cursor) {

    atlr_profile_start_with_id((char *)__func__, 0);
    AtlrJsonValue val = {};
    
    if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACE) {
        val.data = _atlr_parse_object(cursor);
        val.type = ATLR_JSON_VALUE_TYPE_OBJECT;
    } else if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET) {
        val.data = _atlr_parse_array(cursor);
        val.type = ATLR_JSON_VALUE_TYPE_ARRAY;
    } else if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_STRING) {
        val.data = (*cursor)->data;
        val.type = ATLR_JSON_VALUE_TYPE_STRING;
    } else if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_NUMBER) {
        val.data = (*cursor)->data;
        val.type = ATLR_JSON_VALUE_TYPE_NUMBER;
    } else if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_BOOLEAN_TRUE || 
        (*cursor)->type == ATLR_JSON_TOKEN_TYPE_BOOLEAN_FALSE) {
        b32* value = (b32*) malloc(sizeof(b32));
        *value = 0;
        if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_BOOLEAN_TRUE) {
            *value = 1;
        }
        val.data = value;
        val.type = ATLR_JSON_VALUE_TYPE_BOOLEAN;
    } else if ((*cursor)->type == ATLR_JSON_TOKEN_TYPE_NULL) {
        val.data = (*cursor)->data;
        val.type = ATLR_JSON_VALUE_TYPE_NULL;
    } else {
        val.type = ATLR_JSON_VALUE_TYPE_UNKNOWN;
    }
    cursor++;

    atlr_profile_end();
    return val;
}

static AtlrJsonArray* _atlr_make_array(AtlrJsonToken *cursor) {
    AtlrJsonArray *arr = (AtlrJsonArray *) malloc(sizeof(AtlrJsonArray));
    arr->len = 0;

    b32 in_object = 0, in_array = 0;
    u32 open_brackets = 0, open_braces = 0;
    loop {
        cursor++;
        if (!in_object && !in_array) {
            
            if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET) {
                break;
            }

            if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACE) {
                in_object = 1;
                open_braces++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_COMMA) {
                continue;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET) {
                in_array = 1;
                open_brackets++;
            }
            arr->len++;
        } else {

            if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACE) {
                open_braces++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET) {
                open_brackets++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE) {
                open_braces--;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET) {
                open_brackets--;
            }
        }

        if (in_object && open_braces == 0) {
            in_object = 0;
        }
        if (in_array && open_brackets == 0) {
            in_array = 0;
        }
    }
    arr->values = (AtlrJsonValue*) malloc(sizeof(AtlrJsonValue) * arr->len);
    return arr;
}

static AtlrJsonArray* _atlr_parse_array(AtlrJsonToken **tokens) {
    AtlrJsonArray *arr = _atlr_make_array(*tokens);
    u64 count = 0;
    loop {
        (*tokens)++;
        if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_COMMA) {
            count++;
        } else if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            break;
        } else {
            *(arr->values + count) = _atlr_parse_value(tokens);
        }
    }
    return arr;
}

static AtlrJsonObject* _atlr_make_object(AtlrJsonToken *cursor) {
    AtlrJsonObject *object = (AtlrJsonObject *) malloc(sizeof(AtlrJsonObject));
    object->len = 0;

    b32 in_object = 0, in_array = 0;
    u32 open_brackets = 0, open_braces = 0;
    loop {
        cursor++;
        if (!in_object && !in_array) {
            
            if (cursor->type == ATLR_JSON_TOKEN_TYPE_STRING) {
                object->len++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE) {
                break;
            }

            if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACE) {
                in_object = 1;
                open_braces++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET) {
                in_array = 1;
                open_brackets++;
            }
        } else {

            if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACE) {
                open_braces++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET) {
                open_brackets++;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE) {
                open_braces--;
            } else if (cursor->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET) {
                open_brackets--;
            }
        }

        if (in_object && open_braces == 0) {
            in_object = 0;
        }
        if (in_array && open_brackets == 0) {
            in_array = 0;
        }
    }
    object->pairs = (AtlrJsonKeyValuePair*) malloc(sizeof(AtlrJsonKeyValuePair) * object->len);
    for (u64 i = 0; i < object->len; i++) {
        (object->pairs + i)->value = (AtlrJsonValue*) malloc(sizeof(AtlrJsonValue));
    }
    return object;
}

static AtlrJsonObject* _atlr_parse_object(AtlrJsonToken **tokens) {
    atlr_profile_start_with_id((char*) __func__, 0);
    AtlrJsonObject *object = _atlr_make_object(*tokens);
    u64 count = 0;
    loop {
        (*tokens)++;
        if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_STRING) {
            (object->pairs + count)->key = (char*) (*tokens)->data;
            (*tokens)++;
            if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_COLON) {
                (*tokens)++;
                *(object->pairs + count)->value = _atlr_parse_value(tokens);
            } else {
                printf("Not sure where we are\n");
            }
        } else if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_COMMA) {
            count++;
        } else if ((*tokens)->type == ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE) {
            break;
        } else {
            printf("Wut?");
            break;
        }
    }

    atlr_profile_end();
    return object;
}

static char* atlr_json_token_type_to_string(AtlrJsonTokenType type) {
    switch (type) {
        case ATLR_JSON_TOKEN_TYPE_UNKNOWN: return (char *) "Unknown";
        case ATLR_JSON_TOKEN_TYPE_LEFT_BRACE: return (char *) "Left Brace";
        case ATLR_JSON_TOKEN_TYPE_LEFT_BRACKET: return (char *) "Left Bracket";
        case ATLR_JSON_TOKEN_TYPE_RIGHT_BRACE: return (char *) "Right Brace";
        case ATLR_JSON_TOKEN_TYPE_RIGHT_BRACKET: return (char *) "Right Bracket";
        case ATLR_JSON_TOKEN_TYPE_STRING: return (char *) "String";
        case ATLR_JSON_TOKEN_TYPE_NUMBER: return (char *) "Number";
        case ATLR_JSON_TOKEN_TYPE_BOOLEAN_TRUE: return (char *) "True";
        case ATLR_JSON_TOKEN_TYPE_BOOLEAN_FALSE: return (char *) "False";
        case ATLR_JSON_TOKEN_TYPE_COLON: return (char *) "Colon";
        case ATLR_JSON_TOKEN_TYPE_COMMA: return (char *) "Comma";
        case ATLR_JSON_TOKEN_TYPE_NULL: return (char *) "Null";
    }
    return (char *) "Undefined";
}

static char* atlr_json_value_type_to_string(AtlrJsonValueType type) {
    switch (type) {
        case ATLR_JSON_VALUE_TYPE_UNKNOWN: return (char*) "Unknown";
        case ATLR_JSON_VALUE_TYPE_OBJECT: return (char*) "Object";
        case ATLR_JSON_VALUE_TYPE_ARRAY: return (char*) "Array";
        case ATLR_JSON_VALUE_TYPE_STRING: return (char*) "String";
        case ATLR_JSON_VALUE_TYPE_NUMBER: return (char*) "Number";
        case ATLR_JSON_VALUE_TYPE_BOOLEAN: return (char*) "Boolean";
        case ATLR_JSON_VALUE_TYPE_NULL: return (char*) "Null";
    }
    return (char *) "Undefined";
}


static AtlrJsonValue atlr_get_json_from_file(char* file_location) {
    atlr_profile_filter((char*) "tokens");

    // NOTE: repetition testing
    {
        AtlrProfileRepetition repetition = atlr_profile_repetition();
        loop {
            u32 fd_json = open(file_location, O_RDONLY); 
            FileStat meta = {};
            u32 result = fstat(fd_json, &meta);
            if (result != 0 || meta.st_size == 0) {
                printf("Could not read json file\n");
                // TODO(torija): handle error
                return (AtlrJsonValue){};
            }

            atlr_profile_init_block(&repetition.current, (char*) "read_file", sizeof(char) * meta.st_size);

            // NOTE: try mmap
            char *json_contents = (char*) malloc(sizeof(char) * meta.st_size);
            read(fd_json, json_contents, sizeof(char) * meta.st_size);
            close(fd_json);
            free(json_contents);

            if (!atlr_profile_repetition_lap(&repetition)) {
                break;
            }
        }
    }

    AtlrFile* json_file = atlr_fs_get_file(file_location);
    atlr_profile_start_with_id((char*) "read_file", json_file->size);
    atlr_fs_load_file(json_file);
    atlr_profile_end();
    if (!json_file->is_loaded) {
        atlr_log_error("Could not read json file\n");
        // TODO(torija): handle error
        return (AtlrJsonValue){};
    }

    atlr_profile_start_with_id((char*) "tokens", sizeof(AtlrJsonToken) * json_file->size);
    // TODO(torija): extremely oversized, do better (；⌣̀_⌣́)
    AtlrJsonToken *tokens = (AtlrJsonToken *) malloc(sizeof(AtlrJsonToken) * json_file->size);
    _atlr_tokenize_string(json_file->data, json_file->size, tokens);
    atlr_profile_end();

    atlr_profile_start_with_id((char*) "parse", 0);
    AtlrJsonValue value = _atlr_parse_value(&tokens);
    atlr_profile_end();

    atlr_profile_print();
    return value;
}

static AtlrJsonValue* atlr_get_value_by_key(AtlrJsonObject *object, char* key) {
    for (u32 i = 0; i < object->len; i++) {
        if (strcmp((object->pairs + i)->key, key) == 0) {
            return (object->pairs + i)->value;
        }
    }
    return NULL;
}

// ===========================================================
// @implementation: atlr_image
// ===========================================================

static AtlrImage atlr_image_load(char* location) {
    s32 w, h, n;
    unsigned char *img_data = stbi_load(location, &w, &h, &n, 4);
    
    return (AtlrImage) {
        .data = (u32*) img_data,
        .width = w,
        .height = h,
        .channels = 4,
    };
}

static u32 atlr_image_get_color(AtlrImage img, s32 x, s32 y) {
    if (img.height < y || img.width < x || x < 0 || y < 0 || ((img.width * y) + x) > (img.width * img.height)) {
        return 0;
    }
    u32 pixel = img.data[(y * img.width) + x];
    u32 alpha = 0xFF & (pixel >> 24);
    u32 red = 0xFF & (pixel >> 16);
    u32 green =  0xFF & (pixel >> 8);
    u32 blue = 0xFF & (pixel);
    return alpha << 24| blue << 16 | green << 8 | red;
}

// ===========================================================
// @implementation: atlr_font
// ===========================================================

static AtlrFont atlr_font_load(char* font_path, f32 font_scale, AtlrArena* arena) {
    AtlrFile* font_file = atlr_fs_get_file(font_path);
    atlr_fs_load_file(font_file);
    AtlrFont atlr_font = {
        .font_file = font_file,
        .scale = font_scale,
    };
    s32 offset = stbtt_GetFontOffsetForIndex((u8*)atlr_font.font_file->data, 0);
    stbtt_InitFont(&atlr_font.font, (u8*)atlr_font.font_file->data, offset);
    atlr_font.pixel_height = stbtt_ScaleForPixelHeight(&atlr_font.font, font_scale);
    atlr_font.atlas = (AtlrFontAtlas) {
        .glyphs = (AtlrFontGlyph*) atlr_mem_allocate(arena, sizeof(AtlrFontGlyph) * atlr_font.font.numGlyphs),
        .capacity = atlr_font.font.numGlyphs,
        .last = 0,
    };
    return atlr_font;
}

static AtlrFontGlyph atlr_font_get_glyph(AtlrFont* font, char codepoint) {
    for (u32 i = 0; i < font->atlas.last; i++) {
        if (codepoint == font->atlas.glyphs[i].codepoint) {
            return font->atlas.glyphs[i];
        }
    }
    s32 w, h, x_off, y_off;
    // TODO: should we keep this bitmap in our mem arena and free this one?
    u8 *bitmap = stbtt_GetCodepointBitmap(&font->font, 0, font->pixel_height, codepoint, &w, &h, &x_off, &y_off);
    s32 x_shift = x_off;
    s32 y_shift = h + y_off;
    font->atlas.glyphs[font->atlas.last] = (AtlrFontGlyph) {
        .codepoint = codepoint,
        .width = w,
        .height = h,
        .x_shift = x_shift,
        .y_shift = y_shift,
        .bitmap = bitmap,
    };
    font->atlas.last++;
    return font->atlas.glyphs[font->atlas.last - 1];
}

// ===========================================================
// @implementation: atlr_rtzr
// ===========================================================

static void atlr_rtzr_draw_label(u32* data, s32 w, s32 h, AtlrString *str, u32 color, Vec2 origin, AtlrFont* font) {
    u32 x_pos = origin.x;
    u32 y_pos = origin.y - font->scale;
    u32 fixed_kern = font->scale / 10;
    for (u32 k = 0; k < str->len; k++) {
        if (str->data[k] == ' ') {
            x_pos += font->scale / 1.5;
            continue;
        }

        AtlrFontGlyph glyph = atlr_font_get_glyph(font, str->data[k]);
        for (s32 j = 0; j < glyph.height; ++j) {
            for (s32 i = 0; i < glyph.width; ++i) {
                if (glyph.bitmap[j * glyph.width + i] > 0) {
                    atlr_rtzr_draw_pixel(data, w, h, i + x_pos + glyph.x_shift, glyph.height - j + y_pos - glyph.y_shift, color);
                }
            }
        }
        x_pos += glyph.width + fixed_kern;
    }
}

static Line atlr_rtzr_interpolate(s32 dep_end, s32 dep_start, s32 ind_end, s32 ind_start) {
    Line line = (Line) {};
    line.count = abs(ind_end - ind_start);
    // TODO: use mem arena
    line.points = (Vec2*) malloc(sizeof(Vec2) * line.count);
    f64 slope = (f64) (dep_end - dep_start) / (f64) (ind_end - ind_start);
    for (s32 i = 0; i < (s32) line.count; i++) {
        line.points[i] = (Vec2) {
            .values = {
                (f64) ind_start + i,
                dep_start + (slope * i)
            }
        };
    }
    return line;
}

static void atlr_rtzr_order_triangle(Triangle *triangle) {
    if (triangle->points[1].y < triangle->points[0].y) {
        Vec2 temp = triangle->points[1];
        triangle->points[1] = triangle->points[0];
        triangle->points[0] = temp;
    }
    if (triangle->points[2].y < triangle->points[0].y) {
        Vec2 temp = triangle->points[2];
        triangle->points[2] = triangle->points[0];
        triangle->points[0] = temp;
    }
    if (triangle->points[2].y < triangle->points[1].y) {
        Vec2 temp = triangle->points[2];
        triangle->points[2] = triangle->points[1];
        triangle->points[1] = temp;
    }
}

static void atlr_rtzr_make_rect_triangles(Rectangle rect, Triangle* triangles) {
    triangles[0] = (Triangle) {
        (Vec2) {.x = (f64)rect.x,          .y = (f64) rect.y}, 
        (Vec2) {.x = (f64)rect.x + rect.w, .y = (f64) rect.y},
        (Vec2) {.x = (f64)rect.x,          .y = (f64) rect.y - rect.h},
    };
    triangles[1] = (Triangle) {
        (Vec2) {.x = (f64) rect.x + rect.w, .y = (f64) rect.y}, 
        (Vec2) {.x = (f64) rect.x,          .y = (f64) rect.y - rect.h},
        (Vec2) {.x = (f64) rect.x + rect.w, .y = (f64) rect.y - rect.h},
    };
}

static void atlr_rtzr_draw_pixel(u32* data, s32 w, s32 h, s32 x, s32 y, u32 color) {
    s32 fb_x = (w / 2) + x;
    s32 fb_y = (h / 2) - y;
    if (fb_y < 0 || fb_y >= h) return;
    if (fb_x < 0 || fb_x >= w) return;
    data[fb_y * w + fb_x] = color;
}

static void atlr_rtzr_draw_line(u32* data, s32 w, s32 h, Vec2 from, Vec2 to, u32 color) {
    if (atlr_algebra_vec2_equal(from, to)) {
        atlr_rtzr_draw_pixel(data, w, h, from.x, from.y, color);
        return;
    }

    Vec2 first_point = from;
    Vec2 last_point = to;
    Vec2 vector = atlr_algebra_vec2_substract(last_point, first_point);

    Line line;
    if (fabs(vector.x) > fabs(vector.y))  {
        if (last_point.x < first_point.x) {
            first_point = to;
            last_point = from;
        }
        line = atlr_rtzr_interpolate(last_point.y, first_point.y, last_point.x, first_point.x);
        for (u32 i = 0; i < line.count; i++) {
            atlr_rtzr_draw_pixel(data, w, h, line.points[i].values[0], line.points[i].values[1], color);
        }
    } else {
        if (last_point.y < first_point.y) {
            first_point = to;
            last_point = from;
        }
        line = atlr_rtzr_interpolate(last_point.x, first_point.x, last_point.y, first_point.y);
        for (u32 i = 0; i < line.count; i++) {
            atlr_rtzr_draw_pixel(data, w, h, line.points[i].values[1], line.points[i].values[0], color);
        }
    }
    free(line.points);
}

static void atlr_rtzr_draw_triangle(u32* data, s32 w, s32 h, Triangle triangle, u32 color) {
    atlr_rtzr_draw_line(data, w, h, triangle.points[0], triangle.points[1],  color);
    atlr_rtzr_draw_line(data, w, h, triangle.points[1], triangle.points[2],  color);
    atlr_rtzr_draw_line(data, w, h, triangle.points[2], triangle.points[0],  color);
}

static void atlr_rtzr_draw_filled_triangle(u32* data, s32 w, s32 h, Triangle triangle, u32 color) {
    atlr_rtzr_order_triangle(&triangle);

    Line line01 = atlr_rtzr_interpolate(triangle.points[1].x, triangle.points[0].x, triangle.points[1].y, triangle.points[0].y);
    Line line12 = atlr_rtzr_interpolate(triangle.points[2].x, triangle.points[1].x, triangle.points[2].y, triangle.points[1].y);
    Line line02 = atlr_rtzr_interpolate(triangle.points[2].x, triangle.points[0].x, triangle.points[2].y, triangle.points[0].y);

    Line line012 = (Line) {
        .points = (Vec2*) realloc(line01.points, sizeof(Vec2) * (line01.count + line12.count)),
        .count = line01.count + line12.count,
    };
    for (u32 i = 0; i < line12.count; i++) {
        line012.points[i + line01.count] = line12.points[i];
    }

    s32 middle = floor((f64)line02.count / 2);
    Line x_left, x_right;
    if (line02.points[middle].values[1] < line012.points[middle].values[1]) {
        x_left = line02; 
        x_right = line012; 
    } else {
        x_left = line012;
        x_right = line02;
    }

    for (s32 y = triangle.points[0].y; y < triangle.points[2].y; y++) {
        s32 position = y - triangle.points[0].y;
        Vec2 left = x_left.points[position];
        Vec2 right = x_right.points[position];
        for (s32 x = left.values[1]; x <= right.values[1]; x++) {
            atlr_rtzr_draw_pixel(data, w, h, x, y, color);
        }
    }
    free(line12.points);
    free(line02.points);
    free(line012.points);
}


static void atlr_rtzr_draw_filled_rectangle(u32* data, s32 w, s32 h, Rectangle rect, u32 color, Matrix2x2 rotation) {
    Triangle t[2];
    atlr_rtzr_make_rect_triangles(rect, t);
    t[0].points[0] = atlr_algebra_cross_m2x2_v2(rotation, t[0].points[0]);
    t[0].points[1] = atlr_algebra_cross_m2x2_v2(rotation, t[0].points[1]);
    t[0].points[2] = atlr_algebra_cross_m2x2_v2(rotation, t[0].points[2]);
    t[1].points[0] = atlr_algebra_cross_m2x2_v2(rotation, t[1].points[0]);
    t[1].points[1] = atlr_algebra_cross_m2x2_v2(rotation, t[1].points[1]);
    t[1].points[2] = atlr_algebra_cross_m2x2_v2(rotation, t[1].points[2]);
    atlr_rtzr_draw_filled_triangle(data, w, h, t[0], color);
    atlr_rtzr_draw_filled_triangle(data, w, h, t[1], color);
}

static void atlr_rtzr_draw_triangle_texture(
    u32* data, s32 w, s32 h,
    Triangle triangle, 
    AtlrImage image,
    u32 rect_w, 
    u32 rect_h,
    b32 right_side,
    Matrix2x2 rotation
) {
    atlr_rtzr_order_triangle(&triangle);

    Line line01 = atlr_rtzr_interpolate(triangle.points[1].x, triangle.points[0].x, triangle.points[1].y, triangle.points[0].y);
    Line line12 = atlr_rtzr_interpolate(triangle.points[2].x, triangle.points[1].x, triangle.points[2].y, triangle.points[1].y);
    Line line02 = atlr_rtzr_interpolate(triangle.points[2].x, triangle.points[0].x, triangle.points[2].y, triangle.points[0].y);

    Line line012 = (Line) {
        .points = (Vec2*) realloc(line01.points, sizeof(Vec2) * (line01.count + line12.count)),
        .count = line01.count + line12.count,
    };
    for (u32 i = 0; i < line12.count; i++) {
        line012.points[i + line01.count] = line12.points[i];
    }

    s32 middle = floor((f64)line02.count / 2);
    Line x_left, x_right;
    if (line02.points[middle].values[1] < line012.points[middle].values[1]) {
        x_left = line02; 
        x_right = line012; 
    } else {
        x_left = line012;
        x_right = line02;
    }

    for (s32 y = triangle.points[0].y; y < triangle.points[2].y - 1; y++) {
        s32 y_pos = y - triangle.points[0].y;
        Vec2 left = x_left.points[y_pos];
        Vec2 right = x_right.points[y_pos];
        f64 v = 0;
        if ((triangle.points[2].y - triangle.points[0].y) != 0) {
            v = (f64) y_pos / (rect_h);
        }
        for (s32 x = left.values[1]; x <= right.values[1]; x++) {
            f64 u = 0;
            u32 x_range = right.values[1] - left.values[1];
            u32 x_pos = (x - left.values[1]);
            if (right_side) {
                x_pos = x_range - x_pos;
            }
            if ((right.values[1] - left.values[1]) != 0) {
                u = (f64) (x_pos) / (rect_w);
            }
            u32 text_x = image.width * u;
            if (right_side) {
                text_x = image.width - text_x;
            }
            u32 text_y = image.height - (image.height * v);

            u32 color = atlr_image_get_color(image, text_x, text_y);
            Vec2 point = atlr_algebra_cross_m2x2_v2(rotation, (Vec2) { .x = (f64)x, .y =(f64) y});
            atlr_rtzr_draw_pixel(data, w, h, point.x, point.y, color);
        }
    }
    free(line12.points);
    free(line02.points);
    free(line012.points);
}

static void atlr_rtzr_draw_image(u32* data, s32 w, s32 h, AtlrImage image, Rectangle dest, Matrix2x2 rotation) {
    Triangle t[2];
    atlr_rtzr_make_rect_triangles(dest, t);

    atlr_rtzr_draw_triangle_texture(data, w, h, t[0], image, dest.w, dest.h, 0, rotation);
    atlr_rtzr_draw_triangle_texture(data, w, h, t[1], image, dest.w, dest.h, 1, rotation);
}

// ===========================================================
// @implementation: atlr_math
// ===========================================================

static f64 atlr_string_to_f64(char* str, u64 len) {
    s64 dot_position = 0;
    b32 negative = 0;
    f64 value = 0;
    u64 current_position = 0;
    for (u64 i = 0; i < len; i++) {
        if (str[i] == '.') {
            dot_position = i;
        }
    }

    if (str[0] == '-') {
        negative = 1;
        current_position++;
        dot_position--;
    }

    for (;current_position < len; current_position++) {
        if (str[current_position] == '.') {
            continue;
        }
        u64 current_position_value = str[current_position] - '0';
        value += current_position_value * pow(10, dot_position - 1);;
        dot_position--;
    }

    if (negative) {
        value *= -1;
    }
    return value;
}

// ===========================================================
// @implementation: algebra
// ===========================================================

static b32 atlr_algebra_vec2_equal(Vec2 a, Vec2 b) {
    if (a.x == b.x && a.y == b.y) {
        return 1;
    }
    return 0;
}

static Vec2 atlr_algebra_vec2_substract(Vec2 a, Vec2 b) {
    return (Vec2) {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

static Vec2 atlr_algebra_cross_m2x2_v2(Matrix2x2 m, Vec2 v) {
    return (Vec2) {
        (m.values[0] * v.values[0]) + (m.values[1] * v.values[1]),
        (m.values[2] * v.values[0]) + (m.values[3] * v.values[1]),
    };
}

static Matrix2x2 atlr_algebra_get_roll_m2x2(f64 degree) {
    f64 radian = degree * ATLR_RADIAN;
    return (Matrix2x2) {
        cos(radian), -sin(radian),
        sin(radian),  cos(radian),
    };
}



#endif

#endif


