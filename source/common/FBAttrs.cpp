#include "FBAttrs.h"
#include <unordered_map>

const std::string _KN_ = "###KN_PLACE_HOLDER###";
const std::string _NM_ = "###NM_PLACE_HOLDER###";
const std::string _DM_ = "###DM_PLACE_HOLDER###";

const std::string chars_kana_default   = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ�������������������������������������������ܦ���";
const std::string padding_kana_default = "R ";

const std::string chars_num_default    = "0123456789";
const std::string padding_num_default  = "L0";

const std::string chars_dummy_default    = " ";
const std::string padding_dummy_default  = "R ";

// SOHFURI
const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "�f�[�^�敪",   "�u�P�v�F�w�b�_�[���R�[�h"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "��ʃR�[�h",   "�u�Q�P�v�F�����U��"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "�R�[�h�敪",   "�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "�˗��l�R�[�h", "�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "�˗��l��",     ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "��g��",       "�l�l�c�c�i�l�l���c�c���j"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "�d����s�ԍ�", "������Z�@�֔ԍ�"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "�d����s��",   ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "�d���x�X�ԍ�", "����X�ԍ�"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "�d���x�X��",   ""},
        {10,  95,  1, 'N', _NM_, 'O', _NM_, "�a�����",     "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
        {11,  96,  7, 'N', _NM_, 'O', _NM_, "�����ԍ�",     ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "�_�~�[",       "�X�y�[�X�Ƃ���"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "�f�[�^�敪",     "�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "��s�ԍ�",       "������Z�@�֔ԍ�"},
        { 2,   5, 15, 'C', _KN_, 'O', _KN_, "��s��",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "�x�X�ԍ�",       "����X�ԍ�"},
        { 4,  23, 15, 'C', _KN_, 'O', _KN_, "�x�X��",         ""},
        { 5,  38,  4, 'N', _NM_, 'O', _NM_, "��`�������ԍ�", "�����`�������ԍ�"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "�a�����",       "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "�����ԍ�",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "���l��",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "�U�����z",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "�V�K�R�[�h",     "�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11,  91, 10, 'N', _KN_, 'O', _KN_, "�ڋq�R�[�h1",    "�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
        {12, 101, 10, 'N', _KN_, 'O', _KN_, "�ڋq�R�[�h2",    "�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
        {13, 111,  1, 'N', _NM_, 'O', _NM_, "�U���敪",       "�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
        {14, 112,  1, 'C', _KN_, 'O', _KN_, "���ʕ\��",       "�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
        {15, 113,  7, 'C', _DM_, '-', _DM_, "�_�~�[",         "�X�y�[�X�Ƃ���"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "�f�[�^�敪", "�u�W�v�F�g���[���[���R�[�h"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "���v����",   ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "���v���z",   ""},
        { 3, 19, 101, 'C', _DM_, '-', _DM_, "�_�~�[",     "�X�y�[�X�Ƃ���"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "�f�[�^�敪", "�u�X�v�F�G���h���R�[�h"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "�_�~�[",     "�X�y�[�X�Ƃ���"}
    }
};

//KYUYO_SHOYO
const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "�f�[�^�敪",         "�u�P�v�F�w�b�_�[���R�[�h"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "��ʃR�[�h",         "�u�P�P�v�F���^�U���i���ԁj\r\n�u�P�Q�v�F���^�U���i���ԁj\r\n�u�V�P�v�F���^�U���i�n���������j\r\n�u�V�Q�v�F�ܗ^�U���i�n���������j"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "�R�[�h�敪",         "�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "��ЃR�[�h",         "��s���̔Ԃ��������̉�ЃR�[�h"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "��Ж�",             ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "�U���w���",         "�l�l�c�c�i�l�l���c�c���j"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "�d����s�ԍ�",       "������Z�@�֔ԍ�"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "�d����s��",         ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "�d���x�X�ԍ�",       "����X�ԍ�"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "�d���x�X��",         ""},
        {10,  95,  1, 'N', _NM_, 'O', _NM_, "�a����ځi��Ɠ��j", "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��"},
        {11,  96,  7, 'N', _NM_, 'O', _NM_, "�����ԍ��i��Ɠ��j", ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "�_�~�[",             "�X�y�[�X�Ƃ���"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "�f�[�^�敪",     "�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "��s�ԍ�",       "������Z�@�֔ԍ�"},
        { 2,   5, 15, 'C', _KN_, 'M', _KN_, "��s��",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "�x�X�ԍ�",       "����X�ԍ�"},
        { 4,  23, 15, 'C', _KN_, 'M', _KN_, "�x�X��",         ""},
        { 5,  38,  4, 'N', _NM_, 'O', _NM_, "��`�������ԍ�", "�����`�������ԍ�"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "�a�����",       "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "�����ԍ�",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "�a���Җ�",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "�U�����z",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "�V�K�R�[�h",     "�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11,  91, 10, 'N', _NM_, 'O', _NM_, "�Ј��ԍ�",       "��Ɠ��ł̎Ј��ԍ�"},
        {12, 101, 10, 'N', _NM_, 'O', _NM_, "�����R�[�h",     "��Ɠ��ł̏����R�[�h"},
        {13, 111,  9, 'C', _DM_, '-', _DM_, "�_�~�[",         "�X�y�[�X�Ƃ���"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "�f�[�^�敪", "�u�W�v�F�g���[���[���R�[�h"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "���v����",   ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "���v���z",   ""},
        { 3, 19, 101, 'C', _DM_, '-', _DM_, "�_�~�[",     "�X�y�[�X�Ƃ���"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "�f�[�^�敪", "�u�X�v�F�G���h���R�[�h"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "�_�~�[",     "�X�y�[�X�Ƃ���"}
    }
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "�f�[�^�敪",         "�u�P�v�F�w�b�_�[���R�[�h"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "��ʃR�[�h",         "�u�X�P�v�F�a�������U��"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "�R�[�h�敪",         "�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b\r\n���̑��̃R�[�h�ɂ��Ă͑S�⋦��ɂ��"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "�ϑ��҃R�[�h",       "��s����߂��ϑ��҂̃R�[�h"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "�ϑ��Җ�",           ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "������",             "�l�l�c�c�i�l�l���c�c���j"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "�����s�ԍ�",       "������Z�@�֔ԍ�"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "�����s��",         ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "����x�X�ԍ�",       "����X�ԍ�"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "����x�X��",         ""},
        {10,  95,  1, 'N', _NM_, 'M', _NM_, "�a����ځi�ϑ��ҁj", "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
        {11,  96,  7, 'N', _NM_, 'M', _NM_, "�����ԍ��i�ϑ��ҁj", ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "�_�~�[",             "�X�y�[�X�Ƃ���"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "�f�[�^�敪",     "�u�Q�v�F�f�[�^�[���R�[�h"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "��s�ԍ�",       "������Z�@�֔ԍ�"},
        { 2,   5, 15, 'C', _KN_, 'O', _KN_, "��s��",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "�x�X�ԍ�",       "����X�ԍ�"},
        { 4,  23, 15, 'C', _KN_, 'O', _KN_, "�x�X��",         ""},
        { 5,  38,  4, 'C', _DM_, '-', _DM_, "�_�~�[",         "�X�y�[�X�Ƃ���"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "�a�����",       "�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�R�v�F�[�ŏ����a��\r\n�u�X�v�F���̑�"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "�����ԍ�",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "�a���Җ�",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "�������z",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "�V�K�R�[�h",     "�u�P�v�F��P�������\r\n�u�Q�v�F�ύX���i��s�E�x�X�E�����ԍ��j\r\n�u�O�v�F���̑�"},
        {11,  91, 20, 'N', _NM_, 'M', _NM_, "�ڋq�ԍ�",       "�ϑ��҂���߂��ڋq�ԍ�"},
        {12, 111,  1, 'N', _NM_, 'M', _NM_, "�U�֌��ʃR�[�h", "�U�֏������s��������\r\n�˗����ׂł́u�O�v�Ƃ���"},
        {13, 112,  8, 'C', _DM_, '-', _DM_, "�_�~�[",         "�X�y�[�X�Ƃ���"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "�f�[�^�敪",   "�u�W�v�F�g���[���[���R�[�h"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "���v����",     ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "���v���z",     ""},
        { 3, 19,   6, 'N', _NM_, 'M', _NM_, "�U�֍ό���",   "�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 4, 25,  12, 'N', _NM_, 'M', _NM_, "�U�֍ϋ��z",   "�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 5, 37,   6, 'N', _NM_, 'M', _NM_, "�U�֕s�\����", "�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 6, 43,  12, 'N', _NM_, 'M', _NM_, "�U�֕s�\���z", "�˗����ׂł͑S�āu�O�v�Ƃ���"},
        { 7, 55,  65, 'C', _DM_, '-', _DM_, "�_�~�[",       "�X�y�[�X�Ƃ���"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "�f�[�^�敪", "�u�X�v�F�G���h���R�[�h"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "�_�~�[",     "�X�y�[�X�Ƃ���"}
    }
};

FBAttrsArray make_attrs_array
(
    FBType type,
    std::string chars_kana,
    std::string padding_kana,
    std::string chars_num,
    std::string padding_num,
    std::string chars_dummy,
    std::string padding_dummy 
)
{
    FBAttrsArray attrs_array;

    switch(type)
    {
        case FBType::SOHFURI:
            attrs_array = ATTRS_ARRAY_SOHFURI;          
            break;

        case FBType::KYUYO_SHOYO:
            attrs_array = ATTRS_ARRAY_KYUYO_SHOYO;  
            break;

        case FBType::FURIKAE:
            attrs_array = ATTRS_ARRAY_FURIKAE;  
            break;

        default:
            throw;          
            break;
    }
    

    if(chars_kana.empty()   ){ chars_kana     = chars_kana_default;    }
    if(padding_kana.empty() ){ padding_kana   = padding_kana_default;  }
    if(chars_num.empty()    ){ chars_num      = chars_num_default;     }
    if(padding_num.empty()  ){ padding_num    = padding_num_default;   }
    if(chars_dummy.empty()  ){ chars_dummy    = chars_dummy_default;   }
    if(padding_dummy.empty()){ padding_dummy  = padding_dummy_default; }

    for(auto& attrs : attrs_array)
    {
        for(auto& attr : attrs)
        {
            if(attr.char_includes == _KN_){ attr.char_includes = chars_kana;    }
            if(attr.padding_conf  == _KN_){ attr.padding_conf  = padding_kana;  }
            if(attr.char_includes == _NM_){ attr.char_includes = chars_num;     }
            if(attr.padding_conf  == _NM_){ attr.padding_conf  = padding_num;   }
            if(attr.char_includes == _DM_){ attr.char_includes = chars_dummy;   }
            if(attr.padding_conf  == _DM_){ attr.padding_conf  = padding_dummy; }

            std::string  descript;
            descript =
                "���ځF" + attr.label + 
                "\r\n" +
                "�����F" + attr.char_kind + "�i" + std::to_string(attr.length) + "�j" +
                "\r\n" +
                "���́F" + (attr.optionality == 'M' ? "�K�{" : (attr.optionality == 'O' ? "�C��" : (attr.optionality == '-' ? "�Ȃ�" : ""))) +
                "\r\n" +
                "\r\n" +
                (!attr.descript.empty() ? "�����F" : "") + 
                "\r\n" +
                attr.descript;


            attr.descript = descript;

        }
        
    }

    return attrs_array;
}