#include "crosscore.hpp"
#include "scene.hpp"
#include "dynrig.hpp"


void Ponytail::init(ScnObj* pObj, const char* pPrefix) {
	mpObj = pObj;
	mNumNodes = 0;
	if (!pObj || !pObj->has_skel() || !pObj->mpMotWk) {
		return;
	}
	sxModelData* pMdl = pObj->get_model_data();
	int nskl = pMdl->mSklNum;
	int ipony = 0;
	char ponyNodeName[32];
	for (int i = 0; i < nskl; ++i) {
		XD_SPRINTF(XD_SPRINTF_BUF(ponyNodeName, sizeof(ponyNodeName)), "%s%d", pPrefix ? pPrefix : "x_Pony", ipony);
		if (nxCore::str_eq(ponyNodeName, pMdl->get_skel_name(i))) {
			mNodes[ipony].mJntId = i;
			mNodes[ipony].mMaxDist = 0.1f;
			mNodes[ipony].mForce = 0.02f;
			mNodes[ipony].mRate = 0.8f;
			mNodes[ipony].mLimitTheta = XD_PI / 2.0f;
			mNodes[ipony].mLimitPhi = XD_PI;
			mNodes[ipony].mAttr = 1;
			++ipony;
		}
	}
	mNumNodes = ipony;
	mNumAdjs = 0;
}

void Ponytail::reset_nodes() {
	int n = mNumNodes;
	if (n <= 0) return;
	if (!mpObj) return;
	for (int i = 0; i < n; ++i) {
		mNodes[i].mAttr |= 1;
		mpObj->reset_skel_local_mtx(mNodes[i].mJntId);
	}
	mNodes[0].mWMtx = mpObj->calc_skel_world_mtx(mNodes[0].mJntId);
	for (int i = 1; i < n; ++i) {
		mNodes[i].mWMtx.mul(mpObj->get_skel_local_mtx(mNodes[i].mJntId), mNodes[i - 1].mWMtx);
	}
	for (int i = 0; i < n; ++i) {
		mNodes[i].mPrevWMtx = mNodes[i].mWMtx;
	}
}

static bool pony_node_adj(Ponytail* pPony, const int inode, const cxVec npos, const cxVec opos, cxVec* pAdjPos) {
	bool flg = false;
	cxVec pos = npos;
	float dist = nxVec::dist(opos, npos);
	if (pPony->mNumAdjs > 0) {
		for (int i = 0; i < pPony->mNumAdjs; ++i) {
			if (pPony->mpObj->get_model_data()->ck_skel_id(pPony->mAdjs[i].mJntId)) {
				cxMtx mtx = pPony->mpObj->calc_skel_world_mtx(pPony->mAdjs[i].mJntId);
				cxSphere sph = pPony->mAdjs[i].mSph;
				cxVec sc = mtx.calc_pnt(sph.get_center());
				float rr = nxCalc::sq(sph.get_radius());
				cxVec v0 = pos - opos;
				if (v0.mag2() < rr) {
					v0 = sc - opos;
					float len = v0.mag();
					if (len == 0.0f) {
						pos = opos;
					} else {
						float c = (rr - nxCalc::sq(dist) - nxCalc::sq(len)) / (-2.0f * dist * len);
						c = nxCalc::clamp(c, -0.9999f, 0.9999f);
						float ang = ::acosf(c);
						cxVec v1 = pos - opos;
						cxVec axis = nxVec::cross(v0, v1);
						mtx.set_rot(axis, ang);
						v0.normalize();
						v0.scl(dist);
						v0 = mtx.calc_vec(v0);
						pos = opos + v0;
					}
					flg = true;
				}
			}
		}
	}
	*pAdjPos = pos;
	return flg;
}

static void pony_node_move(Ponytail* pPony, const int inode, bool objFlg = !true) {
	Ponytail::Node* pNode = &pPony->mNodes[inode];
	if (pNode->mAttr & 8) {
		return;
	}

	cxVec wold = objFlg ? pPony->mpObj->get_skel_prev_world_mtx(pNode->mJntId).get_translation() : pNode->mPrevWMtx.get_translation();
	cxVec wnow = pNode->mWMtx.get_translation();

	if (pNode->mAttr & 1) {
		pNode->mAttr &= ~1;
		pNode->mPos = pNode->mWMtx.get_translation();
		pNode->mPos.y -= pNode->mMaxDist;
		pNode->mAdd.zero();
		pNode->mPrevWMtx = pNode->mWMtx;
		wnow = pNode->mWMtx.get_translation();
		wold = wnow;
	}

	const float eps = 0.001f;

	cxMtx imtx;
	if (objFlg) {
		imtx = pPony->mpObj->get_skel_prev_world_mtx(pNode->mJntId).get_inverted();
	} else {
		imtx = pNode->mPrevWMtx.get_inverted();
		pNode->mPrevWMtx = pNode->mWMtx;
	}

	cxMtx mtx = imtx * pNode->mWMtx;
	cxVec rpos = mtx.calc_pnt(pNode->mPos);
	if (pNode->mAttr & 2) {
		pNode->mAttr &= ~2;
		pNode->mPos.sub(wold);
		pNode->mPos.add(wnow);
	}

	pNode->mPrevPos = pNode->mPos;

	bool adjFlg = false;
	if (pNode->mAttr & 4) {
		adjFlg = pony_node_adj(pPony, inode, pNode->mPos, wnow, &pNode->mPos);
	}

	cxVec axis;

	pNode->mAdd.y -= pNode->mForce;
	pNode->mPos.add(pNode->mAdd);
	cxVec v0 = pNode->mPos - wnow;
	cxVec v1 = rpos - wnow;
	float dist = v0.mag();
	v0.normalize();
	v1.normalize();
	float c = nxCalc::clamp(v0.dot(v1), -1.0f, 1.0f);
	float th = ::acosf(c);
	if (th > pNode->mLimitTheta) {
		if (th < eps || th >= (XD_PI - eps)) {
			axis.set(v1.z, v1.y, -v1.x);
		} else {
			axis.cross(v1, v0);
		}
		mtx.set_rot(axis, pNode->mLimitTheta);
		v1 = mtx.calc_vec(v1);
		v1.scl(dist);
		pNode->mPos = wnow + v1;
	}

	if (dist > pNode->mMaxDist) {
		v1 = v0 * pNode->mMaxDist;
		pNode->mPos = wnow + v1;
	}

	v1 = pNode->mWMtx.calc_vec(cxVec(0.0f, -1.0f, 0.0f));
	v1.normalize();
	c = nxCalc::clamp(v0.dot(v1), -1.0f, 1.0f);
	float ph = ::acosf(c);
	if (ph > pNode->mLimitPhi) {
		ph = pNode->mLimitPhi;
	}
	if (ph > eps) {
		axis.cross(v1, v0);
		axis.normalize();
		mtx.set_rot(axis, ph);
		pNode->mWMtx.mul(mtx);
		pNode->mWMtx.set_translation(wnow);
	}

	dist = nxVec::dist(pNode->mPos, wnow);
	v0.set(0.0f, -dist, 0.0f);
	pNode->mPos = pNode->mWMtx.calc_pnt(v0);
	pNode->mAdd = pNode->mPos - pNode->mPrevPos;

	float rate = pNode->mRate;
	if (adjFlg) {
		rate *= 0.8f;
	}
	pNode->mAdd.scl(rate);
}

void Ponytail::move() {
	if (!mpObj) return;
	int n = mNumNodes;
	if (n <= 0) return;
	cxMtx baseParentWM;
	mNodes[0].mWMtx = mpObj->calc_skel_world_mtx(mNodes[0].mJntId, &baseParentWM);
	for (int i = 1; i < n; ++i) {
		mNodes[i].mWMtx.mul(mpObj->get_skel_local_mtx(mNodes[i].mJntId), mNodes[i - 1].mWMtx);
	}
	for (int i = 0; i < n; ++i) {
		pony_node_move(this, i);
	}

	for (int i = 0; i < n; ++i) {
		if (mNodes[i].mAttr & 8) continue;
		cxMtx im;
		if (i == 0) {
			im = baseParentWM.get_inverted();
		} else {
			im = mNodes[i - 1].mWMtx.get_inverted();
		}
		cxMtx lm = mNodes[i].mWMtx * im;
		//nxMtx::clean_rotations(&lm, 1);
		mpObj->set_skel_local_mtx(mNodes[i].mJntId, lm);
	}
}


void LegInfo::init(const ScnObj* pObj, const char side, const bool ext) {
	char jname[64];
	XD_SPRINTF(XD_SPRINTF_BUF(jname, sizeof(jname)), "j_Hip_%c", side);
	inodeTop = pObj->find_skel_node_id(jname);
	XD_SPRINTF(XD_SPRINTF_BUF(jname, sizeof(jname)), "j_Knee_%c", side);
	inodeRot = pObj->find_skel_node_id(jname);
	XD_SPRINTF(XD_SPRINTF_BUF(jname, sizeof(jname)), "j_Ankle_%c", side);
	inodeEnd = pObj->find_skel_node_id(jname);
	if (ext) {
		XD_SPRINTF(XD_SPRINTF_BUF(jname, sizeof(jname)), "j_Toe_%c", side);
		inodeExt = pObj->find_skel_node_id(jname);
	} else {
		inodeExt = -1;
	}
	inodeEff = inodeExt < 0 ? inodeEnd : inodeExt;
	effY = pObj->calc_skel_world_rest_mtx(inodeEff).get_translation().y - 0.01f;
	if (ext) {
		cxVec effOffs = pObj->get_skel_local_rest_pos(inodeEff) * 0.5f;
		effOffsX = effOffs.x;
		effOffsZ = effOffs.z;
	} else {
		effOffsX = 0.0f;
		effOffsZ = 0.0f;
	}
}


namespace DynRig {

void calc_forearm_twist(ScnObj* pObj, const int wristId, const int forearmId, const float wristInfluence) {
	if (!pObj) return;
	if (!pObj->ck_skel_id(wristId)) return;
	if (!pObj->ck_skel_id(forearmId)) return;
	cxQuat wristQ = pObj->get_skel_local_quat(wristId);
	cxQuat forearmQ = pObj->get_skel_local_rest_quat(forearmId);
	forearmQ = nxQuat::slerp(wristQ.get_closest_x(), forearmQ, wristInfluence);
	pObj->set_skel_local_quat(forearmId, forearmQ);
}

void calc_forearm_twist(ScnObj* pObj, const char* pWristName, const char* pForearmName, const float wristInfluence) {
	if (!pObj) return;
	calc_forearm_twist(pObj, pObj->find_skel_node_id(pWristName), pObj->find_skel_node_id(pForearmName), wristInfluence);
}

void calc_forearm_twist_l(ScnObj* pObj, const float wristInfluence) {
	calc_forearm_twist(pObj, "j_Wrist_L", "s_Forearm_L", wristInfluence);
}

void calc_forearm_twist_r(ScnObj* pObj, const float wristInfluence) {
	calc_forearm_twist(pObj, "j_Wrist_R", "s_Forearm_R", wristInfluence);
}

void reset_shoulder_rot(ScnObj* pObj, const int shoulderId) {
	if (!pObj) return;
	pObj->reset_skel_local_quat(shoulderId);
}

void reset_shoulder_rot(ScnObj* pObj, const char* pName) {
	reset_shoulder_rot(pObj, pObj->find_skel_node_id(pName));
}

void reset_shoulder_rot_l(ScnObj* pObj) {
	reset_shoulder_rot(pObj, "s_Shoulder_L");
}

void reset_shoulder_rot_r(ScnObj* pObj) {
	reset_shoulder_rot(pObj, "s_Shoulder_R");
}

void calc_shoulder_axis_rot(ScnObj* pObj, const int shoulderJntId, const int shoulderId, const float shoulderJntInfluence, const int axisIdx, const bool useQuats) {
	if (shoulderJntInfluence == 0.0f) return;
	if (!pObj) return;
	if (!pObj->ck_skel_id(shoulderJntId)) return;
	if (!pObj->ck_skel_id(shoulderId)) return;
	cxQuat shoulderJntQ = pObj->get_skel_local_quat(shoulderJntId);
	cxQuat shoulderQ = pObj->get_skel_local_quat(shoulderId);
	if (useQuats) {
		switch (axisIdx) {
			case 0:
				shoulderQ = nxQuat::slerp(shoulderQ, shoulderJntQ.get_closest_x(), shoulderJntInfluence);
				break;
			case 1:
				shoulderQ = nxQuat::slerp(shoulderQ, shoulderJntQ.get_closest_y(), shoulderJntInfluence);
				break;
			case 2:
			default:
				shoulderQ = nxQuat::slerp(shoulderQ, shoulderJntQ.get_closest_z(), shoulderJntInfluence);
				break;
		}
	} else {
		cxVec shoulderJntR = shoulderJntQ.get_rot_degrees();
		cxVec shoulderR = shoulderQ.get_rot_degrees();
		switch (axisIdx) {
			case 0:
				shoulderR.x = nxCalc::lerp(shoulderR.x, shoulderJntR.x, shoulderJntInfluence);
				break;
			case 1:
				shoulderR.y = nxCalc::lerp(shoulderR.y, shoulderJntR.y, shoulderJntInfluence);
				break;
			case 2:
			default:
				shoulderR.z = nxCalc::lerp(shoulderR.z, shoulderJntR.z, shoulderJntInfluence);
				break;
		}
		shoulderQ.set_rot_degrees(shoulderR);
	}
	pObj->set_skel_local_quat(shoulderId, shoulderQ);
}


void calc_shoulder_axis_rot(ScnObj* pObj, const char* pShoulderJntName, const char* pShoulderName, const float shoulderJntInfluence, const int axisIdx) {
	if (!pObj) return;
	calc_shoulder_axis_rot(pObj, pObj->find_skel_node_id(pShoulderJntName), pObj->find_skel_node_id(pShoulderName), shoulderJntInfluence, axisIdx);
}

void calc_shoulder_axis_rot_l(ScnObj* pObj, const float shoulderJntInfluence, const int axisIdx) {
	calc_shoulder_axis_rot(pObj, "j_Shoulder_L", "s_Shoulder_L", shoulderJntInfluence, axisIdx);
}

void calc_shoulder_axis_rot_r(ScnObj* pObj, const float shoulderJntInfluence, const int axisIdx) {
	calc_shoulder_axis_rot(pObj, "j_Shoulder_R", "s_Shoulder_R", shoulderJntInfluence, axisIdx);
}

void calc_eyelids_blink(ScnObj* pObj, const float yopen, const float yclosed, const float t, const float p1, const float p2) {
	if (!pObj) return;
	int eyelidL = pObj->find_skel_node_id("f_Eyelid_L");
	int eyelidR = pObj->find_skel_node_id("f_Eyelid_R");
	float tloop = t * 2.0f;
	if (t > 0.5f) {
		tloop = 2.0f - tloop;
	}
	float val = nxCalc::ease_crv(p1, p2, tloop);
	float y = nxCalc::fit(val, 0.0f, 1.0f, yopen, yclosed);
	pObj->set_skel_local_ty(eyelidL, y);
	pObj->set_skel_local_ty(eyelidR, y);
}

void adjust_leg(ScnObj* pObj, sxCollisionData* pCol, LegInfo* pLeg) {
	if (!pObj) return;
	if (!pCol) return;
	if (!pLeg) return;
	cxMotionWork* pMotWk = pObj->mpMotWk;
	if (!pMotWk) return;
	cxMtx effW = pMotWk->calc_node_world_mtx(pLeg->inodeEff);
	cxVec legPos = effW.get_translation();
	cxVec ckOffs = effW.calc_vec(cxVec(-pLeg->effOffsX, 0.0f, -pLeg->effOffsZ));
	cxVec legUp = legPos + ckOffs;
	cxVec legDn = legUp;
	legUp.y += 0.1f;
	legDn.y -= 0.5f;
	cxLineSeg seg(legUp, legDn);
	sxCollisionData::NearestHit hit = pCol->nearest_hit(seg);
	if (hit.count > 0) {
		float hy = hit.pos.y + pLeg->effY;
		float ay = legPos.y;
		if (hy > legPos.y) {
			ay = hy;
		}
		cxVec effPos(legPos.x, ay, legPos.z);
		pMotWk->adjust_leg(effPos, pLeg->inodeTop, pLeg->inodeRot, pLeg->inodeEnd, pLeg->inodeExt);
	}
}

} // DynRig

