#include "FBParser.h"

constexpr char char_num []  = "0123456789";
constexpr char pad_num []   = "L0";

constexpr char char_kana []  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ�����������������������������������������������";
constexpr char pad_kana []   = "R ";

constexpr char char_dummy []  = " ";
constexpr char pad_dummy []   = "R ";


//���U
const FBAttrs FBParser::fb_header_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",     0,  1, "1",        "L1",      nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
    { 1, "��ʃR�[�h",     1,  2, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�Q�j\r\n�K�{����\r\n�u�Q�P�v�F�����U��"},
    { 2, "�R�[�h�敪",     3,  1, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
    { 3, "�˗��l�R�[�h",   4, 10, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�O�j\r\n�K�{����\r\n�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
    { 4, "�˗��l��",      14, 40, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�S�O�j\r\n�K�{����"},
    { 5, "��g��",        54,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
    { 6, "�d����s�ԍ�",  58,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 7, "�d����s��",    62, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 8, "�d���x�X�ԍ�",  77,  3, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 9, "�d���x�X��",    80, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    {10, "�a�����",      95,  1, char_num,   pad_num,   " ",       "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
    {11, "�����ԍ�",      96,  7, char_num,   pad_num,   "       ", "������i���j�F�m�i�V�j\r\n�C�Ӎ���"},
    {12, "�_�~�[",       103, 17, char_dummy, pad_dummy, nullptr,   "������i���j�F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_data_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",      0,  1, "2",        "L2",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
    { 1, "��s�ԍ�",        1,  4, char_num,   pad_num,   nullptr, "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 2, "��s��",          5, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 3, "�x�X�ԍ�",       20,  3, char_num,   pad_num,   nullptr, "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 4, "�x�X��",         23, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 5, "��`�������ԍ�", 38,  4, char_num,   pad_num,   "    ",  "������i���j�F�m�i�S�j\r\n �C�Ӎ���\r\n�����`�������ԍ�"},
    { 6, "�a�����",       42,  1, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
    { 7, "�����ԍ�",       43,  7, char_num,   pad_num,   nullptr, "������i���j�F�m�i�V�j\r\n�K�{����"},
    { 8, "���l��",       50, 30, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�R�O�j\r\n�K�{����"},
    { 9, "�U�����z",       80, 10, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�O�j\r\n�K�{����"},
    {10, "�V�K�R�[�h",     90,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
    {11, "�ڋq�R�[�h1",    91, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {12, "�ڋq�R�[�h2",   101, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {13, "�U���敪",      111,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
    {14, "���ʕ\\��",     112,  1, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�j\r\n�C�Ӎ���\r\n�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
    {15, "�_�~�[",        113,  7, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_trailer_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",  0,   1, "8",        "L8",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
    { 1, "���v����",    1,   6, char_num,   pad_num,   nullptr, "������i���j�F�m�i�U�j\r\n�K�{����"},
    { 2, "���v���z",    7,  12, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�Q�j\r\n�K�{����"},
    { 3, "�_�~�[",     19, 101, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_end_attrs_sohfuri =
{
    { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
    { 1, "�_�~�[",     1, 119, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

//���^������

const FBAttrs FBParser::fb_header_attrs_kyuyo =
{
    { 0, "�f�[�^�敪",     0,  1, "1",        "L1",      nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
    { 1, "��ʃR�[�h",     1,  2, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�Q�j\r\n�K�{����\r\n�u�Q�P�v�F�����U��"},
    { 2, "�R�[�h�敪",     3,  1, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
    { 3, "�˗��l�R�[�h",   4, 10, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�O�j\r\n�K�{����\r\n�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
    { 4, "�˗��l��",      14, 40, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�S�O�j\r\n�K�{����"},
    { 5, "��g��",        54,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
    { 6, "�d����s�ԍ�",  58,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 7, "�d����s��",    62, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 8, "�d���x�X�ԍ�",  77,  3, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 9, "�d���x�X��",    80, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    {10, "�a�����",      95,  1, char_num,   pad_num,   " ",       "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
    {11, "�����ԍ�",      96,  7, char_num,   pad_num,   "       ", "������i���j�F�m�i�V�j\r\n�C�Ӎ���"},
    {12, "�_�~�[",       103, 17, char_dummy, pad_dummy, nullptr,   "������i���j�F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_data_attrs_kyuyo =
{
    { 0, "�f�[�^�敪",      0,  1, "2",        "L2",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
    { 1, "��s�ԍ�",        1,  4, char_num,   pad_num,   nullptr, "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 2, "��s��",          5, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 3, "�x�X�ԍ�",       20,  3, char_num,   pad_num,   nullptr, "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 4, "�x�X��",         23, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 5, "��`�������ԍ�", 38,  4, char_num,   pad_num,   "    ",  "������i���j�F�m�i�S�j\r\n �C�Ӎ���\r\n�����`�������ԍ�"},
    { 6, "�a�����",       42,  1, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
    { 7, "�����ԍ�",       43,  7, char_num,   pad_num,   nullptr, "������i���j�F�m�i�V�j\r\n�K�{����"},
    { 8, "���l��",       50, 30, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�R�O�j\r\n�K�{����"},
    { 9, "�U�����z",       80, 10, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�O�j\r\n�K�{����"},
    {10, "�V�K�R�[�h",     90,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
    {11, "�ڋq�R�[�h1",    91, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {12, "�ڋq�R�[�h2",   101, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {13, "�U���敪",      111,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
    {14, "���ʕ\\��",     112,  1, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�j\r\n�C�Ӎ���\r\n�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
    {15, "�_�~�[",        113,  7, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_trailer_attrs_kyuyo =
{
    { 0, "�f�[�^�敪",  0,   1, "8",        "L8",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
    { 1, "���v����",    1,   6, char_num,   pad_num,   nullptr, "������i���j�F�m�i�U�j\r\n�K�{����"},
    { 2, "���v���z",    7,  12, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�Q�j\r\n�K�{����"},
    { 3, "�_�~�[",     19, 101, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_end_attrs_kyuyo =
{
    { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
    { 1, "�_�~�[",     1, 119, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

//�ܗ^�i�������j

const FBAttrs FBParser::fb_header_attrs_shoyo =
{
    { 0, "�f�[�^�敪",     0,  1, "1",        "L1",      nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
    { 1, "��ʃR�[�h",     1,  2, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�Q�j\r\n�K�{����\r\n�u�Q�P�v�F�����U��"},
    { 2, "�R�[�h�敪",     3,  1, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
    { 3, "�˗��l�R�[�h",   4, 10, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�O�j\r\n�K�{����\r\n�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
    { 4, "�˗��l��",      14, 40, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�S�O�j\r\n�K�{����"},
    { 5, "��g��",        54,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
    { 6, "�d����s�ԍ�",  58,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 7, "�d����s��",    62, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 8, "�d���x�X�ԍ�",  77,  3, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 9, "�d���x�X��",    80, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    {10, "�a�����",      95,  1, char_num,   pad_num,   " ",       "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
    {11, "�����ԍ�",      96,  7, char_num,   pad_num,   "       ", "������i���j�F�m�i�V�j\r\n�C�Ӎ���"},
    {12, "�_�~�[",       103, 17, char_dummy, pad_dummy, nullptr,   "������i���j�F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_data_attrs_shoyo =
{
    { 0, "�f�[�^�敪",      0,  1, "2",        "L2",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
    { 1, "��s�ԍ�",        1,  4, char_num,   pad_num,   nullptr, "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 2, "��s��",          5, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 3, "�x�X�ԍ�",       20,  3, char_num,   pad_num,   nullptr, "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 4, "�x�X��",         23, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 5, "��`�������ԍ�", 38,  4, char_num,   pad_num,   "    ",  "������i���j�F�m�i�S�j\r\n �C�Ӎ���\r\n�����`�������ԍ�"},
    { 6, "�a�����",       42,  1, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
    { 7, "�����ԍ�",       43,  7, char_num,   pad_num,   nullptr, "������i���j�F�m�i�V�j\r\n�K�{����"},
    { 8, "���l��",       50, 30, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�R�O�j\r\n�K�{����"},
    { 9, "�U�����z",       80, 10, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�O�j\r\n�K�{����"},
    {10, "�V�K�R�[�h",     90,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
    {11, "�ڋq�R�[�h1",    91, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {12, "�ڋq�R�[�h2",   101, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {13, "�U���敪",      111,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
    {14, "���ʕ\\��",     112,  1, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�j\r\n�C�Ӎ���\r\n�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
    {15, "�_�~�[",        113,  7, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_trailer_attrs_shoyo =
{
    { 0, "�f�[�^�敪",  0,   1, "8",        "L8",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
    { 1, "���v����",    1,   6, char_num,   pad_num,   nullptr, "������i���j�F�m�i�U�j\r\n�K�{����"},
    { 2, "���v���z",    7,  12, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�Q�j\r\n�K�{����"},
    { 3, "�_�~�[",     19, 101, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBParser::fb_end_attrs_shoyo =
{
    { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
    { 1, "�_�~�[",     1, 119, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};