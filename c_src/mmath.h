#ifdef SOLARIS
#include <sys/byteorder.h>
#endif

#define IS_SET(v) ((v & 0x00000000000000FFLL) != 0)

// Convert from dalmatiner binary format to erlang number
#define FROM_DDB(v) ((ErlNifSInt64) htonll((v & 0x0000000000000100LL) ? ((v & 0xFFFFFFFFFFFFFF00LL) | 0x00000000000000FFLL) : (v & 0xFFFFFFFFFFFFFF00LL)))

// Convert erlang number to dalmatiner exchange format (why ntoh ??)
#define TO_DDB(v) ((ntohll(v) & 0xFFFFFFFFFFFFFF00LL) | 0x0000000000000001LL)


#define GET_CHUNK(chunk)                                                \
  if (!enif_get_int64(env, argv[1], &chunk))                            \
    return enif_make_badarg(env);                                       \
  if (chunk < 1)                                                        \
    return enif_make_badarg(env)

#define GET_BIN(pos, bin, count, vs)              \
  if (!enif_inspect_binary(env, argv[pos], &bin)) \
    return enif_make_badarg(env);                 \
  if (bin.size % sizeof(ErlNifSInt64))            \
    return enif_make_badarg(env);                 \
  count = bin.size / sizeof(ErlNifSInt64);        \
  vs = (ErlNifSInt64 *) bin.data


typedef ErlNifSInt64 ddb_number;
typedef struct {
    int exponent;           // Should be limited to 8b signed
    long long coefficient;  // Should be limited to 48b signed
} dec;

ddb_number dec_to_ddb_number(dec v);

dec dec_from_ddb_number(ddb_number v);
dec dec_from_int64(ErlNifSInt64 v);
dec dec_from_double(double v);
//dec dec_from_string(char *); // TODO

//dec dec_add(dec v1, dec v2);
//dec dec_sub(dec v1, dec v2);
//dec dec_mul(dec v, long m);
//dec dec_div(dec v, long m);
