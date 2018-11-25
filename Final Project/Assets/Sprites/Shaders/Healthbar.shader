// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Healthbar" {
	Properties
	{
		_MainTex("Texture", 2D) = "white"
		_Point("current health percent", float) = 1
		_XScale("scaling of X axis of object", float) = 1
	}

		SubShader
	{
		Tags
		{
			"Queue" = "Transparent"
			"PreviewType" = "Plane"
		}

		Pass
		{
			Blend SrcAlpha OneMinusSrcAlpha

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			uniform float _Point;
			uniform float _XScale;

			struct vertexOutput
			{
				float4 pos : SV_POSITION;
				float4 scale : TEXCOORD0;
			};

			vertexOutput vert(float4 vertexPos : POSITION)
			{
				vertexOutput output;
				output.pos = UnityObjectToClipPos(vertexPos);
				output.scale = (vertexPos / _XScale) + float4(0.5, 0.5, 0.5, 0);
				return output;
			}

			float4 frag(vertexOutput input) : COLOR
			{
				float4 color;
				if ((input.scale.x + 9.1) / 19.1 > _Point)
				//if ((input.scale.x + 1.5) / 4 > _Point)
				{
					color = float4(1, 0, 0, 1);
				}
				else {
					color = float4(0, 1, 0, 1);
				}
				return color;
			}

			ENDCG
			

			/*struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
			};

			v2f vert(appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}

			sampler2D _MainTex;

			float4 frag(v2f i) : SV_Target
			{
				float4 color = tex2D(_MainTex, i.uv) * float4(1, 0, 0, 1);
				//float4 (i.uv.r, i.uv.g, 0, 1);
				return color;
			}
			ENDCG*/
		}
	}
}
