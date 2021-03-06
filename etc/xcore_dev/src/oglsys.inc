#ifdef OGL_FN_CORE
OGL_FN(VIEWPORT, Viewport)
OGL_FN(SCISSOR, Scissor)
OGL_FN(GETINTEGERV, GetIntegerv)
OGL_FN(GETSTRING, GetString)
OGL_FN(LINEWIDTH, LineWidth)
OGL_FN(COLORMASK, ColorMask)
OGL_FN(DEPTHMASK, DepthMask)
OGL_FN(DEPTHFUNC, DepthFunc)
OGL_FN(STENCILMASK, StencilMask)
OGL_FN(STENCILOP, StencilOp)
OGL_FN(STENCILFUNC, StencilFunc)
OGL_FN(STENCILMASKSEPARATE, StencilMaskSeparate)
OGL_FN(STENCILOPSEPARATE, StencilOpSeparate)
OGL_FN(STENCILFUNCSEPARATE, StencilFuncSeparate)
OGL_FN(CLEARCOLOR, ClearColor)
OGL_FN(CLEARSTENCIL, ClearStencil)
OGL_FN(CLEAR, Clear)
OGL_FN(ENABLE, Enable)
OGL_FN(DISABLE, Disable)
OGL_FN(FRONTFACE, FrontFace)
OGL_FN(CULLFACE, CullFace)
OGL_FN(BLENDEQUATION, BlendEquation)
OGL_FN(BLENDFUNC, BlendFunc)
OGL_FN(BLENDEQUATIONSEPARATE, BlendEquationSeparate)
OGL_FN(BLENDFUNCSEPARATE, BlendFuncSeparate)
OGL_FN(GENBUFFERS, GenBuffers)
OGL_FN(BINDBUFFER, BindBuffer)
OGL_FN(BUFFERDATA, BufferData)
OGL_FN(BUFFERSUBDATA, BufferSubData)
OGL_FN(BINDBUFFERRANGE, BindBufferRange)
OGL_FN(DELETEBUFFERS, DeleteBuffers)
OGL_FN(GENSAMPLERS, GenSamplers)
OGL_FN(DELETESAMPLERS, DeleteSamplers)
OGL_FN(SAMPLERPARAMETERI, SamplerParameteri)
OGL_FN(BINDSAMPLER, BindSampler)
OGL_FN(GENTEXTURES, GenTextures)
OGL_FN(DELETETEXTURES, DeleteTextures)
OGL_FN(BINDTEXTURE, BindTexture)
OGL_FN(TEXIMAGE2D, TexImage2D)
OGL_FN(TEXSTORAGE2D, TexStorage2D)
OGL_FN(TEXSTORAGE3D, TexStorage3D)
OGL_FN(TEXSUBIMAGE3D, TexSubImage3D)
OGL_FN(TEXPARAMETERI, TexParameteri)
OGL_FN(ACTIVETEXTURE, ActiveTexture)
OGL_FN(GENERATEMIPMAP, GenerateMipmap)
OGL_FN(PIXELSTOREI, PixelStorei)
OGL_FN(CREATESHADER, CreateShader)
OGL_FN(SHADERSOURCE, ShaderSource)
OGL_FN(COMPILESHADER, CompileShader)
OGL_FN(DELETESHADER, DeleteShader)
OGL_FN(GETSHADERIV, GetShaderiv)
OGL_FN(GETSHADERINFOLOG, GetShaderInfoLog)
OGL_FN(ATTACHSHADER, AttachShader)
OGL_FN(DETACHSHADER, DetachShader)
OGL_FN(CREATEPROGRAM, CreateProgram)
OGL_FN(LINKPROGRAM, LinkProgram)
OGL_FN(DELETEPROGRAM, DeleteProgram)
OGL_FN(USEPROGRAM, UseProgram)
OGL_FN(GETPROGRAMIV, GetProgramiv)
OGL_FN(GETPROGRAMINFOLOG, GetProgramInfoLog)
OGL_FN(GETUNIFORMLOCATION, GetUniformLocation)
OGL_FN(UNIFORM1I, Uniform1i)
OGL_FN(UNIFORM1F, Uniform1f)
OGL_FN(UNIFORM1FV, Uniform1fv)
OGL_FN(UNIFORM2FV, Uniform2fv)
OGL_FN(UNIFORM3FV, Uniform3fv)
OGL_FN(UNIFORM4FV, Uniform4fv)
OGL_FN(UNIFORMMATRIX4FV, UniformMatrix4fv)
OGL_FN(GETATTRIBLOCATION, GetAttribLocation)
OGL_FN(GETUNIFORMBLOCKINDEX, GetUniformBlockIndex)
OGL_FN(UNIFORMBLOCKBINDING, UniformBlockBinding)
OGL_FN(VERTEXATTRIBPOINTER, VertexAttribPointer)
OGL_FN(ENABLEVERTEXATTRIBARRAY, EnableVertexAttribArray)
OGL_FN(DISABLEVERTEXATTRIBARRAY, DisableVertexAttribArray)
OGL_FN(VERTEXATTRIB4FV, VertexAttrib4fv)
OGL_FN(VERTEXATTRIB4IV, VertexAttrib4iv)
OGL_FN(BINDBUFFERBASE, BindBufferBase)
OGL_FN(DRAWARRAYS, DrawArrays)
OGL_FN(DRAWELEMENTS, DrawElements)
OGL_FN(GENFRAMEBUFFERS, GenFramebuffers)
OGL_FN(DELETEFRAMEBUFFERS, DeleteFramebuffers)
OGL_FN(BINDFRAMEBUFFER, BindFramebuffer)
OGL_FN(FRAMEBUFFERTEXTURE2D, FramebufferTexture2D)
OGL_FN(FRAMEBUFFERRENDERBUFFER, FramebufferRenderbuffer)
OGL_FN(CHECKFRAMEBUFFERSTATUS, CheckFramebufferStatus)
OGL_FN(BLITFRAMEBUFFER, BlitFramebuffer)
OGL_FN(GENRENDERBUFFERS, GenRenderbuffers)
OGL_FN(DELETERENDERBUFFERS, DeleteRenderbuffers)
OGL_FN(BINDRENDERBUFFER, BindRenderbuffer)
OGL_FN(RENDERBUFFERSTORAGE, RenderbufferStorage)
#endif /* OGL_FN_CORE */

#ifdef OGL_FN_EXTRA
OGL_FN(GETACTIVEUNIFORMBLOCKIV, GetActiveUniformBlockiv)
OGL_FN(GETUNIFORMINDICES, GetUniformIndices)
OGL_FN(GETACTIVEUNIFORMSIV, GetActiveUniformsiv)
OGL_FN(MAPBUFFERRANGE, MapBufferRange)
OGL_FN(UNMAPBUFFER, UnmapBuffer)
OGL_FN(DRAWBUFFER, DrawBuffer)
OGL_FN(DRAWBUFFERS, DrawBuffers)
OGL_FN(CLEARBUFFERFI, ClearBufferfi)
OGL_FN(CLEARBUFFERFV, ClearBufferfv)
OGL_FN(SHADERBINARY, ShaderBinary)
OGL_FN(GENVERTEXARRAYS, GenVertexArrays)
OGL_FN(DELETEVERTEXARRAYS, DeleteVertexArrays)
OGL_FN(BINDVERTEXARRAY, BindVertexArray)
OGL_FN(DRAWELEMENTSBASEVERTEX, DrawElementsBaseVertex)
OGL_FN(VERTEXATTRIBDIVISOR, VertexAttribDivisor)
OGL_FN(SPECIALIZESHADER, SpecializeShader)
OGL_FN(PROGRAMPARAMETERI, ProgramParameteri)
OGL_FN(GETPROGRAMBINARY, GetProgramBinary)
OGL_FN(GETPROGRAMINTERFACEIV, GetProgramInterfaceiv)
OGL_FN(GETPROGRAMRESOURCEIV, GetProgramResourceiv)
OGL_FN(GETPROGRAMRESOURCEINDEX, GetProgramResourceIndex)
OGL_FN(GETPROGRAMRESOURCELOCATION, GetProgramResourceLocation)
OGL_FN(GENQUERIES, GenQueries)
OGL_FN(DELETEQUERIES, DeleteQueries)
OGL_FN(QUERYCOUNTER, QueryCounter)
OGL_FN(GETQUERYOBJECTIV, GetQueryObjectiv)
OGL_FN(GETQUERYOBJECTUIV, GetQueryObjectuiv)
OGL_FN(GETQUERYOBJECTUI64V, GetQueryObjectui64v)
OGL_FN(COMPRESSEDTEXIMAGE2D, CompressedTexImage2D)
OGL_FN(DISPATCHCOMPUTE, DispatchCompute)
OGL_FN(MEMORYBARRIER, MemoryBarrier)
#endif /* OGL_FN_EXTRA */
