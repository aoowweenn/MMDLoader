#include <cassert>
#include <cstring> // memcpy
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <iostream>

#include "pmd_reader.h"

#define MAX_BUF_LEN 20

struct sjis_table_rec_t
{
  char unicode_name[MAX_BUF_LEN];
  char ascii_name[MAX_BUF_LEN];
  char sjis_name[MAX_BUF_LEN];
};

using namespace mmd;

// http://akemiwhy.deviantart.com/art/mmd-reference-japanese-bone-names-430962605
// http://ash.jp/code/unitbl21.htm
sjis_table_rec_t sjis_table[] = {
    {"グルーブ", "groove", {(char)0x83, (char)0x4F, (char)0x83, (char)0x8B, (char)0x81, (char)0x5B, (char)0x83, (char)0x75, (char)0x00}},
    {"センター", "center", {(char)0x83, (char)0x5A, (char)0x83, (char)0x93, (char)0x83, (char)0x5E, (char)0x81, (char)0x5B, (char)0x00}},
    {"上半身", "upper_body", {(char)0x8F, (char)0xE3, (char)0x94, (char)0xBC, (char)0x90, (char)0x67, (char)0x00}},
    {"下半身", "lower_body", {(char)0x89, (char)0xBA, (char)0x94, (char)0xBC, (char)0x90, (char)0x67, (char)0x00}},
    {"両目", "eyes", {(char)0x97, (char)0xBC, (char)0x96, (char)0xDA, (char)0x00}},
    {"全ての親", "mother", {(char)0x91, (char)0x53, (char)0x82, (char)0xC4, (char)0x82, (char)0xCC, (char)0x90, (char)0x65, (char)0x00}},
    {"右つま先ＩＫ", "toe_IK_R", {(char)0x89, (char)0x45, (char)0x82, (char)0xC2, (char)0x82, (char)0xDC, (char)0x90, (char)0xE6, (char)0x82, (char)0x68, (char)0x82, (char)0x6A, (char)0x00}},
    {"右ひざ", "knee_R", {(char)0x89, (char)0x45, (char)0x82, (char)0xD0, (char)0x82, (char)0xB4, (char)0x00}},
    {"右ひじ", "elbow_R", {(char)0x89, (char)0x45, (char)0x82, (char)0xD0, (char)0x82, (char)0xB6, (char)0x00}},
    {"右中指１", "middle1_R", {(char)0x89, (char)0x45, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"右中指２", "middle2_R", {(char)0x89, (char)0x45, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"右中指３", "middle3_R", {(char)0x89, (char)0x45, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"右人指１", "fore1_R", {(char)0x89, (char)0x45, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"右人指２", "fore2_R", {(char)0x89, (char)0x45, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"右人指３", "fore3_R", {(char)0x89, (char)0x45, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"右小指１", "little1_R", {(char)0x89, (char)0x45, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"右小指２", "little2_R", {(char)0x89, (char)0x45, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"右小指３", "little3_R", {(char)0x89, (char)0x45, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"右手首", "wrist_R", {(char)0x89, (char)0x45, (char)0x8E, (char)0xE8, (char)0x8E, (char)0xF1, (char)0x00}},
    {"右目", "eye_R", {(char)0x89, (char)0x45, (char)0x96, (char)0xDA, (char)0x00}},
    {"右肩", "shoulder_R", {(char)0x89, (char)0x45, (char)0x8C, (char)0xA8, (char)0x00}},
    {"右腕", "arm_R", {(char)0x89, (char)0x45, (char)0x98, (char)0x72, (char)0x00}},
    {"右薬指１", "third1_R", {(char)0x89, (char)0x45, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"右薬指２", "third2_R", {(char)0x89, (char)0x45, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"右薬指３", "third3_R", {(char)0x89, (char)0x45, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"右袖", "sleeve_R", {(char)0x89, (char)0x45, (char)0x91, (char)0xb3, (char)0x00}},
    {"右袖先", "cuff_R", {(char)0x89, (char)0x45, (char)0x91, (char)0xb3, (char)0x90, (char)0xe6, (char)0x00}},
    {"右親指１", "thumb1_R", {(char)0x89, (char)0x45, (char)0x90, (char)0x65, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"右親指２", "thumb2_R", {(char)0x89, (char)0x45, (char)0x90, (char)0x65, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"右足", "leg_R", {(char)0x89, (char)0x45, (char)0x91, (char)0xAB, (char)0x00}},
    {"右足首", "ankle_R", {(char)0x89, (char)0x45, (char)0x91, (char)0xAB, (char)0x8E, (char)0xF1, (char)0x00}},
    {"右足ＩＫ", "leg_IK_R", {(char)0x89, (char)0x45, (char)0x91, (char)0xAB, (char)0x82, (char)0x68, (char)0x82, (char)0x6A, (char)0x00}},
    {"左つま先ＩＫ", "toe_IK_L", {(char)0x8D, (char)0xB6, (char)0x82, (char)0xC2, (char)0x82, (char)0xDC, (char)0x90, (char)0xE6, (char)0x82, (char)0x68, (char)0x82, (char)0x6A, (char)0x00}},
    {"左ひざ", "knee_L", {(char)0x8D, (char)0xB6, (char)0x82, (char)0xD0, (char)0x82, (char)0xB4, (char)0x00}},
    {"左ひじ", "elbow_L", {(char)0x8D, (char)0xB6, (char)0x82, (char)0xD0, (char)0x82, (char)0xB6, (char)0x00}},
    {"左中指１", "middle1_L", {(char)0x8D, (char)0xB6, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"左中指２", "middle2_L", {(char)0x8D, (char)0xB6, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"左中指３", "middle3_L", {(char)0x8D, (char)0xB6, (char)0x92, (char)0x86, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"左人指１", "fore1_L", {(char)0x8D, (char)0xB6, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"左人指２", "fore2_L", {(char)0x8D, (char)0xB6, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"左人指３", "fore3_L", {(char)0x8D, (char)0xB6, (char)0x90, (char)0x6C, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"左小指１", "little1_L", {(char)0x8D, (char)0xB6, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"左小指２", "little2_L", {(char)0x8D, (char)0xB6, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"左小指３", "little3_L", {(char)0x8D, (char)0xB6, (char)0x8F, (char)0xAC, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"左手首", "wrist_L", {(char)0x8D, (char)0xB6, (char)0x8E, (char)0xE8, (char)0x8E, (char)0xF1, (char)0x00}},
    {"左目", "eye_L", {(char)0x8D, (char)0xB6, (char)0x96, (char)0xDA, (char)0x00}},
    {"左肩", "shoulder_L", {(char)0x8D, (char)0xB6, (char)0x8C, (char)0xA8, (char)0x00}},
    {"左腕", "arm_L", {(char)0x8D, (char)0xB6, (char)0x98, (char)0x72, (char)0x00}},
    {"左薬指１", "third1_L", {(char)0x8D, (char)0xB6, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"左薬指２", "third2_L", {(char)0x8D, (char)0xB6, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"左薬指３", "third3_L", {(char)0x8D, (char)0xB6, (char)0x96, (char)0xF2, (char)0x8E, (char)0x77, (char)0x82, (char)0x52, (char)0x00}},
    {"左袖", "sleeve_L", {(char)0x8D, (char)0xB6, (char)0x91, (char)0xb3, (char)0x00}},
    {"左袖先", "cuff_L", {(char)0x8D, (char)0xB6, (char)0x91, (char)0xb3, (char)0x90, (char)0xe6, (char)0x00}},
    {"左親指１", "thumb1_L", {(char)0x8D, (char)0xB6, (char)0x90, (char)0x65, (char)0x8E, (char)0x77, (char)0x82, (char)0x50, (char)0x00}},
    {"左親指２", "thumb2_L", {(char)0x8D, (char)0xB6, (char)0x90, (char)0x65, (char)0x8E, (char)0x77, (char)0x82, (char)0x51, (char)0x00}},
    {"左足", "leg_L", {(char)0x8D, (char)0xB6, (char)0x91, (char)0xAB, (char)0x00}},
    {"左足首", "ankle_L", {(char)0x8D, (char)0xB6, (char)0x91, (char)0xAB, (char)0x8E, (char)0xF1, (char)0x00}},
    {"左足ＩＫ", "leg_IK_L", {(char)0x8D, (char)0xB6, (char)0x91, (char)0xAB, (char)0x82, (char)0x68, (char)0x82, (char)0x6A, (char)0x00}},
    {"頭", "head", {(char)0x93, (char)0xAA, (char)0x00}},
    {"首", "neck", {(char)0x8E, (char)0xF1, (char)0x00}},
    };

typedef std::map<std::string, sjis_table_rec_t*> sjis_map_t;
sjis_map_t sjis_map;

std::set<std::string> unvisited_bones;

//
//
//
static bool ReadBytes(unsigned char *out_data, int size, FILE *fp) {
  size_t sz;
  sz = fread(out_data, 1, size, fp);
  assert(sz == size);
  return true;
}

static bool ParseBone(PMDModel *model, FILE *fp) {
  unvisited_bones.clear();
  for(sjis_map_t::iterator p = sjis_map.begin(); p != sjis_map.end(); p++) {
    unvisited_bones.insert((*p).second->ascii_name);
  }

  // cp932 encoding of 'leg'(hi-za) in Japanese
  const char kLegName[4 + 1] = {(char)0x82, (char)0xd0, (char)0x82, (char)0xb4, (char)0x00}; // +1 for \0

  unsigned short numBones;
  ReadBytes(reinterpret_cast<unsigned char *>(&numBones),
            sizeof(unsigned short), fp);
  printf("[PMD] Num bones: %d\n", numBones);
  assert(sizeof(PMDBone) == 39);
  std::vector<PMDBone> pmdBones(numBones);
  ReadBytes(reinterpret_cast<unsigned char *>(&(pmdBones[0])),
            sizeof(PMDBone) * numBones, fp);

  model->bones_.clear();

  // Bone with name containing 'leg' in Japanese need special treatment
  // when computing IK.
  for (int i = 0; i < numBones; i++) {
    Bone bone;

    bone.parentIndex = pmdBones[i].parent_bone_index;
    bone.tailIndex = pmdBones[i].tail_bone_index;
    bone.type = pmdBones[i].bone_type;
    bone.parentIndexIK = pmdBones[i].ik_parent_bone_index;

    // printf("[PMD] [%d] parent %d, tail %d, ty %d, ik %d\n", i,
    // pmdBones[i].parent_bone_index, pmdBones[i].tail_bone_index,
    // pmdBones[i].bone_type, pmdBones[i].ik_parent_bone_index);

    if (pmdBones[i].tail_bone_index == (unsigned short)(-1)) {
      // skip
      printf("[PMD] Bone [%d] is the tail. Skipping.\n", i);
      continue;
    }

    bone.pos[0] = pmdBones[i].bone_pos[0];
    bone.pos[1] = pmdBones[i].bone_pos[1];
    bone.pos[2] = pmdBones[i].bone_pos[2];
    bone.pos[3] = 1.0f;

    char buf[21];
    memcpy(buf, pmdBones[i].bone_name, 20);
    buf[20] = '\0'; // add '\0' for safety
    bone.name = std::string(buf);
    if (bone.name.find(kLegName) != std::string::npos) {
      printf("[PMD] Bone [%d] is leg\n", i);
      bone.isLeg = true;
    } else {
      bone.isLeg = false;
    }

    sjis_map_t::iterator p = sjis_map.find(buf);
    if (p != sjis_map.end()) {
      bone.ascii_name = (*p).second->ascii_name;
      std::set<std::string>::iterator q = unvisited_bones.find((*p).second->ascii_name);
      if(q != unvisited_bones.end()) {
        unvisited_bones.erase(q);
      }
    }

    model->bones_.push_back(bone);
  }

  for(std::set<std::string>::iterator r = unvisited_bones.begin(); r != unvisited_bones.end(); r++) {
    std::cout << "[PMD] Cannot find bone [ " << *r << " ] in PMD." << std::endl;
  }

  return true;
}

static bool ParseMorph(PMDModel *model, FILE *fp){
  unsigned short numMorphs;
  ReadBytes(reinterpret_cast<unsigned char *>(&numMorphs), sizeof(unsigned short),
            fp);
  printf("[PMD] Num Morphs: %d\n", numMorphs);           
  
  std::vector<Morph> morphs(numMorphs);
  for (int i = 0; i < numMorphs; i++) {
    PMDMorph pmdMorph;
    ReadBytes(reinterpret_cast<unsigned char *>(&pmdMorph), sizeof(PMDMorph), fp);
    std::vector<PMDMorphVertex> vertices(pmdMorph.vertex_count);
    for (int j = 0; j < pmdMorph.vertex_count; j++){
    ReadBytes(reinterpret_cast<unsigned char *>(&vertices[j]), sizeof(PMDMorphVertex), fp);
  }
  }
  return true;
}

static bool ParseIK(PMDModel *model, FILE *fp) {
  unsigned short numIKs;
  ReadBytes(reinterpret_cast<unsigned char *>(&numIKs), sizeof(unsigned short),
            fp);
  printf("[PMD] Num IKs: %d\n", numIKs);
  assert(sizeof(PMDIK) == 11);
  std::vector<IK> iks(numIKs);
  for (int i = 0; i < numIKs; i++) {
    PMDIK pmdIK;
    ReadBytes(reinterpret_cast<unsigned char *>(&pmdIK), sizeof(PMDIK), fp);

    iks[i].boneIndex = pmdIK.bone_index;
    iks[i].targetBoneIndex = pmdIK.target_bone_index;
    iks[i].chainLength = pmdIK.chain_length;
    iks[i].iterations = pmdIK.iterations;
    iks[i].weight = pmdIK.weight;

    iks[i].childBoneIndices.resize(iks[i].chainLength);
    ReadBytes(reinterpret_cast<unsigned char *>(&(iks[i].childBoneIndices[0])),
              sizeof(unsigned short) * iks[i].chainLength, fp);
  }

  model->iks_ = iks;

  return true;
}

//
//
//

PMDReader::PMDReader() {
  size_t n = sizeof(sjis_table)/sizeof(sjis_table_rec_t);
  for(int i = 0; i < n; i++) {
    // DBG
    // wchar_t* unicode_name = (wchar_t*)sjis_table[i].unicode_name;
    // char* ascii_name = sjis_table[i].ascii_name;
    // wchar_t* sjis_name = (wchar_t*)sjis_table[i].sjis_name;
    // printf("unicode_name: %ls, ascii_name: %s, sjis_name: %ls\n", unicode_name, ascii_name, sjis_name);
    sjis_map.insert(sjis_map_t::value_type(sjis_table[i].sjis_name, &sjis_table[i]));
  }
}

PMDReader::~PMDReader() {}

PMDModel *PMDReader::LoadFromFile(const std::string &filename) {
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    fprintf(stderr, "Can't read PMD file [ %s ]\n", filename.c_str());
    return NULL;
  }

  PMDModel *model = new PMDModel();

  // file header
  {
    const char kMagic[] = "Pmd";
    const float kVersion = 1.0f; // (char)0x3F800000

    char magic[3];
    ReadBytes(reinterpret_cast<unsigned char *>(magic), 3, fp);
    assert(magic[0] == kMagic[0]);
    assert(magic[1] == kMagic[1]);
    assert(magic[2] == kMagic[2]);

    float version = 0.0f;
    ReadBytes(reinterpret_cast<unsigned char *>(&version), 4, fp);
    assert(version == kVersion);

    model->version_ = version;
  }

  // name&comment
  {
    unsigned char name[20];
    unsigned char comment[256];
    ReadBytes(name, 20, fp);
    ReadBytes(comment, 256, fp);

    model->name_ = std::string(reinterpret_cast<char *>(name));
    model->comment_ = std::string(reinterpret_cast<char *>(comment));

    printf("[PMDReader] name = %s\n", model->name_.c_str());
    printf("[PMDReader] comment = %s\n", model->comment_.c_str());
  }

  // Vertices
  {
    int numVertices;
    ReadBytes(reinterpret_cast<unsigned char *>(&numVertices), 4, fp);
    printf("[PMD] Num vertices: %d\n", numVertices);
    assert(sizeof(PMDVertex) == 38);
    model->vertices_.resize(numVertices);
    ReadBytes(reinterpret_cast<unsigned char *>(&(model->vertices_[0])),
              sizeof(PMDVertex) * numVertices, fp);
  }

  // Indices
  {
    int numIndices;
    ReadBytes(reinterpret_cast<unsigned char *>(&numIndices), 4, fp);
    printf("[PMD] Num indices: %d\n", numIndices);
    model->indices_.resize(numIndices);
    ReadBytes(reinterpret_cast<unsigned char *>(&(model->indices_[0])),
              sizeof(unsigned short) * numIndices, fp);

    // validate
    for (int i = 0; i < numIndices; i++) {
      assert(model->indices_[i] < model->vertices_.size());
    }
  }

  // Materials
  {
    int numMaterials;
    ReadBytes(reinterpret_cast<unsigned char *>(&numMaterials), 4, fp);
    printf("[PMD] Num materials: %d\n", numMaterials);
    assert(sizeof(PMDMaterial) == 70);
    model->materials_.resize(numMaterials);
    ReadBytes(reinterpret_cast<unsigned char *>(&(model->materials_[0])),
              sizeof(PMDMaterial) * numMaterials, fp);

    // validate
    size_t sumVertexCount = 0;
    for (int i = 0; i < numMaterials; i++) {
      assert((model->materials_[i].vertex_count % 3) == 0);
      sumVertexCount += model->materials_[i].vertex_count;

      // DBG
      // printf("mat[%d] texname = %s\n", i,
      // model->materials_[i].texture_filename);
    }
    assert(sumVertexCount == model->indices_.size());
  }

  assert(ParseBone(model, fp));
  assert(ParseIK(model, fp));
  assert(ParseMorph(model, fp));

  fclose(fp);

  printf("[PMD] Load OK\n");

  return model;
}
