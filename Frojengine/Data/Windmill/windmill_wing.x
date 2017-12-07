xof 0303txt 0032

template Frame {
  <3d82ab46-62da-11cf-ab39-0020af71e433>
  [...]
}

template Matrix4x4 {
  <f6f23f45-7686-11cf-8f52-0040333594a3>
  array FLOAT matrix[16];
}

template FrameTransformMatrix {
  <f6f23f41-7686-11cf-8f52-0040333594a3>
  Matrix4x4 frameMatrix;
}

template Vector {
  <3d82ab5e-62da-11cf-ab39-0020af71e433>
  FLOAT x;
  FLOAT y;
  FLOAT z;
}

template MeshFace {
  <3d82ab5f-62da-11cf-ab39-0020af71e433>
  DWORD nFaceVertexIndices;
  array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
  <3d82ab44-62da-11cf-ab39-0020af71e433>
  DWORD nVertices;
  array Vector vertices[nVertices];
  DWORD nFaces;
  array MeshFace faces[nFaces];
  [...]
}

template MeshNormals {
  <f6f23f43-7686-11cf-8f52-0040333594a3>
  DWORD nNormals;
  array Vector normals[nNormals];
  DWORD nFaceNormals;
  array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
  <f6f23f44-7686-11cf-8f52-0040333594a3>
  FLOAT u;
  FLOAT v;
}

template MeshTextureCoords {
  <f6f23f40-7686-11cf-8f52-0040333594a3>
  DWORD nTextureCoords;
  array Coords2d textureCoords[nTextureCoords];
}

template ColorRGBA {
  <35ff44e0-6c7c-11cf-8f52-0040333594a3>
  FLOAT red;
  FLOAT green;
  FLOAT blue;
  FLOAT alpha;
}

template IndexedColor {
  <1630b820-7842-11cf-8f52-0040333594a3>
  DWORD index;
  ColorRGBA indexColor;
}

template MeshVertexColors {
  <1630b821-7842-11cf-8f52-0040333594a3>
  DWORD nVertexColors;
  array IndexedColor vertexColors[nVertexColors];
}

template VertexElement {
  <f752461c-1e23-48f6-b9f8-8350850f336f>
  DWORD Type;
  DWORD Method;
  DWORD Usage;
  DWORD UsageIndex;
}

template DeclData {
  <bf22e553-292c-4781-9fea-62bd554bdd93>
  DWORD nElements;
  array VertexElement Elements[nElements];
  DWORD nDWords;
  array DWORD data[nDWords];
}

Frame DXCC_ROOT {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 1.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 1.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 1.000000;;
  }

  Frame windmill.obj {
    FrameTransformMatrix {
       1.000000, 0.000000, -0.000000, 0.000000,
      0.000000, 1.000000, -0.000000, 0.000000,
      -0.000000, -0.000000, 1.000000, -0.000000,
      0.000000, 0.000000, -0.000000, 1.000000;;
    }

	Frame windmill_wing {
      FrameTransformMatrix {
         1.000000, 0.000000, -0.000000, 0.000000,
        0.000000, 1.000000, -0.000000, 0.000000,
        -0.000000, -0.000000, 1.000000, -0.000000,
        0.000000, 0.000000, -0.000000, 1.000000;;
      }

      Mesh windmill_wing {
        16;
        -2.000000;0.500000;0.001000;,
        -2.000000;-0.500000;0.001000;,
        0.000000;0.500000;0.001000;,
        0.000000;-0.500000;0.001000;,
        0.000000;0.500000;0.001000;,
        0.000000;-0.500000;0.001000;,
        2.000000;0.500000;0.001000;,
        2.000000;-0.500000;0.001000;,
        -0.500000;2.000000;0.000000;,
        -0.500000;0.000000;0.000000;,
        0.500000;2.000000;0.000000;,
        0.500000;0.000000;0.000000;,
        -0.500000;0.000000;0.000000;,
        -0.500000;-2.000000;0.000000;,
        0.500000;0.000000;0.000000;,
        0.500000;-2.000000;0.000000;;
        8;
        3;2,1,0;,
        3;1,2,3;,
        3;6,5,4;,
        3;5,6,7;,
        3;10,9,8;,
        3;9,10,11;,
        3;14,13,12;,
        3;13,14,15;;

        MeshMaterialList {
          1;
          3;
          0, 0, 0;
          Material {
            1.0; 1.0; 1.0; 1.000000;;
            1.000000;
            0.000000; 0.000000; 0.000000;;
            0.000000; 0.000000; 0.000000;;
            TextureFilename { "windmill.dds"; }
          }
        }

        MeshNormals {
        16;
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;,
        0.000000;0.000000;-1.000000;;
        8;
        3;2,1,0;,
        3;1,2,3;,
        3;6,5,4;,
        3;5,6,7;,
        3;10,9,8;,
        3;9,10,11;,
        3;14,13,12;,
        3;13,14,15;;
        }

        MeshTextureCoords {
        16;
        0.570000;0.984400;,
        0.740000;0.984400;,
        0.570000;0.454900;,
        0.740000;0.454900;,
        0.740000;0.454900;,
        0.570000;0.454900;,
        0.740000;0.984400;,
        0.570000;0.984400;,
        0.740000;0.984400;,
        0.740000;0.454900;,
        0.570000;0.984400;,
        0.570000;0.454900;,
        0.570000;0.454900;,
        0.570000;0.984400;,
        0.740000;0.454900;,
        0.740000;0.984400;;
        }
      }

    }

  }

}
