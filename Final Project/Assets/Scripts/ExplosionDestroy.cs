using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExplosionDestroy : MonoBehaviour {

    // Use this for initialization
    private void FixedUpdate()
    {
        this.gameObject.SetActive(true);
        Destroy(gameObject, 1.0f);
    }
}
