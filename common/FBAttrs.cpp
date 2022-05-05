#include "FBAttrs.h"

const char CHARS_NUM[]   = "0123456789";
const char PAD_NUM[]     = "L0";

const char CHARS_KANA[]  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ�������������������������������������������ܦ���";
const char PAD_KANA[]    = "R ";

const char CHARS_DUMMY[] = " ";
const char PAD_DUMMY[]   = "R ";

//SOHFURI
const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    FBAttrs{//HEADER
        { 0, "�f�[�^�敪",     0,  1, "1",         "L1",      nullptr,   "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
        { 1, "��ʃR�[�h",     1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�Q�j�@\r\n�K�{����\r\n�u�Q�P�v�F�����U��"},
        { 2, "�R�[�h�敪",     3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
        { 3, "�˗��l�R�[�h",   4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�P�O�j\r\n�K�{����\r\n�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
        { 4, "�˗��l��",      14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�S�O�j\r\n�K�{����"},
        { 5, "��g��",        54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�S�j�@\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
        { 6, "�d����s�ԍ�",  58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 7, "�d����s��",    62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 8, "�d���x�X�ԍ�",  77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 9, "�d���x�X��",    80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        {10, "�a�����",      95,  1, CHARS_NUM,   PAD_NUM,   " ",       "���������F�m�i�P�j�@\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
        {11, "�����ԍ�",      96,  7, CHARS_NUM,   PAD_NUM,   "       ", "���������F�m�i�V�j�@\r\n�C�Ӎ���"},
        {12, "�_�~�[",       103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "���������F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "�f�[�^�敪",      0,  1, "2",         "L2",      nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1, "��s�ԍ�",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 2, "��s��",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 3, "�x�X�ԍ�",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 4, "�x�X��",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 5, "��`�������ԍ�", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",  "���������F�m�i�S�j�@\r\n�C�Ӎ���\r\n�����`�������ԍ�"},
        { 6, "�a�����",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
        { 7, "�����ԍ�",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�V�j�@\r\n�K�{����"},
        { 8, "���l��",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�R�O�j\r\n�K�{����"},
        { 9, "�U�����z",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�O�j\r\n�K�{����"},
        {10, "�V�K�R�[�h",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11, "�ڋq�R�[�h1",    91, 10, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�m�i�P�O�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
        {12, "�ڋq�R�[�h2",   101, 10, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�m�i�P�O�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
        {13, "�U���敪",      111,  1, CHARS_NUM,   PAD_NUM,   " ",     "���������F�m�i�P�j�@\r\n�C�Ӎ���\r\n�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
        {14, "���ʕ\��",      112,  1, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�j�@\r\n�C�Ӎ���\r\n�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
        {15, "�_�~�[",        113,  7, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�V�j�@\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "�f�[�^�敪",  0,   1, "8",         "L8",      nullptr, "���������F�m�i�P�j�@�@\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
        { 1, "���v����",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�U�j�@�@\r\n�K�{����"},
        { 2, "���v���z",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�Q�j�@\r\n�K�{����"},
        { 3, "�_�~�[",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "���������F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
        { 1, "�_�~�[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//END
};

//KYUYO_SHOYO

const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    FBAttrs{//HEADER
        { 0, "�f�[�^�敪",          0,  1, "1",         "L1",      nullptr,   "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
        { 1, "��ʃR�[�h",          1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�Q�j�@\r\n�K�{����\r\n�u�P�P�v�F���^�U���i���ԁj\r\n�u�P�Q�v�F���^�U���i���ԁj\r\n�u�V�P�v�F���^�U���i�n���������j\r\n�u�V�Q�v�F�ܗ^�U���i�n���������j"},
        { 2, "�R�[�h�敪",          3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
        { 3, "��ЃR�[�h",          4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�P�O�j\r\n�K�{����\r\n��s���̔Ԃ��������̉�ЃR�[�h"},
        { 4, "��Ж�",             14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�S�O�j\r\n�K�{����"},
        { 5, "�U���w���",         54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�S�j�@\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
        { 6, "�d����s�ԍ�",       58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 7, "�d����s��",         62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 8, "�d���x�X�ԍ�",       77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 9, "�d���x�X��",         80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        {10, "�a����ځi��Ɠ��j", 95,  1, CHARS_NUM,   PAD_NUM,   " ",       "���������F�m�i�P�j�@\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��"},
        {11, "�����ԍ��i��Ɠ��j", 96,  7, CHARS_NUM,   PAD_NUM,   "       ", "���������F�m�i�V�j�@\r\n�C�Ӎ���"},
        {12, "�_�~�[",            103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "���������F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "�f�[�^�敪",      0,  1, "2",         "L2",      nullptr,      "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1, "��s�ԍ�",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 2, "��s��",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "���������F�b�i�P�T�j\r\n�K�{����"},
        { 3, "�x�X�ԍ�",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 4, "�x�X��",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "���������F�b�i�P�T�j\r\n�K�{����"},
        { 5, "��`�������ԍ�", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",       "���������F�m�i�S�j�@\r\n�C�Ӎ���\r\n�����`�������ԍ�"},
        { 6, "�a�����",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��"},
        { 7, "�����ԍ�",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�V�j�@\r\n�K�{����"},
        { 8, "�a���Җ�",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr,      "���������F�b�i�R�O�j\r\n�K�{����"},
        { 9, "�U�����z",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�P�O�j\r\n�K�{����"},
        {10, "�V�K�R�[�h",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11, "�Ј��ԍ�",       91, 10, CHARS_NUM,   PAD_NUM,   "          ", "���������F�m�i�P�O�j\r\n�C�Ӎ���\r\n��Ɠ��ł̎Ј��ԍ�"},
        {12, "�����R�[�h",    101, 10, CHARS_NUM,   PAD_NUM,   "          ", "���������F�m�i�P�O�j\r\n�C�Ӎ���\r\n��Ɠ��ł̏����R�[�h"},
        {13, "�_�~�[",        111,  9, CHARS_DUMMY, PAD_DUMMY, nullptr,      "���������F�b�i�X�j�@\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "�f�[�^�敪",  0,   1, "8",         "L8",      nullptr, "���������F�m�i�P�j�@�@\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
        { 1, "���v����",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�U�j�@�@\r\n�K�{����"},
        { 2, "���v���z",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�Q�j�@\r\n�K�{����"},
        { 3, "�_�~�[",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "���������F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
        { 1, "�_�~�[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//END
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    FBAttrs{//HEADER
        { 0, "�f�[�^�敪",           0,  1, "1",         "L1",      nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
        { 1, "��ʃR�[�h",           1,  2, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�Q�j�@\r\n�K�{����\r\n�u�X�P�v�F�a�������U��"},
        { 2, "�R�[�h�敪",           3,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b\r\n���̑��̃R�[�h�ɂ��Ă͑S�⋦��ɂ��"},
        { 3, "�ϑ��҃R�[�h",         4, 10, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�O�j\r\n�K�{����\r\n��s����߂��ϑ��҂̃R�[�h"},
        { 4, "�ϑ��Җ�",            14, 40, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�S�O�j\r\n�K�{����"},
        { 5, "������",              54,  4, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�S�j�@\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
        { 6, "�����s�ԍ�",        58,  4, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 7, "�����s��",          62, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 8, "����x�X�ԍ�",        77,  3, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 9, "����x�X��",          80, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        {10, "�a����ځi�ϑ��ҁj",  95,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
        {11, "�����ԍ��i�ϑ��ҁj",  96,  7, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�V�j�@\r\n�K�{����"},
        {12, "�_�~�[",             103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "�f�[�^�敪",       0,  1, "2",         "L2",      nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1, "��s�ԍ�",         1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�S�j�@\r\n�K�{����\r\n������Z�@�֔ԍ�"},
        { 2, "��s��",           5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 3, "�x�X�ԍ�",        20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�R�j�@\r\n�K�{����\r\n����X�ԍ�"},
        { 4, "�x�X��",          23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�P�T�j\r\n�C�Ӎ���"},
        { 5, "�_�~�[",          38,  4, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�S�j�@\r\n�K�{����\r\n�X�y�[�X�Ƃ���"},
        { 6, "�a�����",        42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�R�v�F�[�ŏ����a��\r\n�u�X�v�F���̑�"},
        { 7, "�����ԍ�",        43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�V�j�@\r\n�K�{����"},
        { 8, "�a���Җ�",        50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "���������F�b�i�R�O�j\r\n�K�{����"},
        { 9, "�������z",        80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�O�j\r\n�K�{����"},
        {10, "�V�K�R�[�h",      90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�P�v�F��P�������\r\n�u�Q�v�F�ύX���i��s�E�x�X�E�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11, "�ڋq�ԍ�",        91, 20, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�Q�O�j\r\n�K�{����\r\n�ϑ��҂���߂��ڋq�ԍ�"},
        {12, "�U�֌��ʃR�[�h", 111,  1, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�U�֏������s��������\r\n�˗����ׂł́u�O�v�Ƃ���"},
        {13, "�_�~�[",         112,  8, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�W�j�@\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "�f�[�^�敪",    0,   1, "8",         "L8",      nullptr, "���������F�m�i�P�j�@\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
        { 1, "���v����",      1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�U�j�@\r\n�K�{����"},
        { 2, "���v���z",      7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�Q�j\r\n�K�{����"},
        { 3, "�U�֍ό���",   19,   6, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�U�j�@\r\n�K�{����\r\n�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 4, "�U�֍ϋ��z",   25,  12, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�Q�j\r\n�K�{����\r\n�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 5, "�U�֕s�\����", 37,   6, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�U�j�@\r\n�K�{����\r\n�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 6, "�U�֕s�\���z", 43,  12, CHARS_NUM,   PAD_NUM,   nullptr, "���������F�m�i�P�Q�j\r\n�K�{����\r\n�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 7, "�_�~�[",       55,  65, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�T�T�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "���������F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
        { 1, "�_�~�[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "���������F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
    }//END
};

