using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Destructibles : MonoBehaviour {

    public AudioSource source;
    private Animator animator;
    public static AudioClip damageSound, destroySound;
    public Healthbar healthbar;
    public bool isDestroyable;

    public GameObject explosion;
    public Transform crate;

	// Use this for initialization
	void Start () {
        damageSound = Resources.Load<AudioClip>("enemytakedamage");
        destroySound = Resources.Load<AudioClip>("crateexplode");
        source = GetComponent<AudioSource>();
	}

    // Update is called once per frame
	void Update () {
      
    }

    void OnTriggerEnter2D(Collider2D collisionInfo)
    {
        if (isDestroyable)
        {
            if (collisionInfo.gameObject.tag == "Energy Ball")
            {
                healthbar.currentHealth--;
                if (healthbar.currentHealth > 0)
                {
                    source.PlayOneShot(damageSound);
                }
            }

            if (healthbar.currentHealth <= 0)
            {

                if (crate != null) { 
                    Instantiate(explosion, crate.position, transform.rotation = Quaternion.identity);
                    explosion.SetActive(true);
                }
                source.PlayOneShot(destroySound);
                Destroy(this.gameObject);
            }
        }
    }
}
